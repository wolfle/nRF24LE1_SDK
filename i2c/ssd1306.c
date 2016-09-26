/*

[!] SSD1306 Display Memory 101
Buffer is 8kbit (1kB) addressed with two auto-incrementable pointers as 8 pages * (128 columns * 8 bits)

         Display Memory        Column
     Cols 0    ->   127       
		+---------------+       +---+
  PAGE0 | XCCCC...CCCCC |       | 0 |
R PAGE1 | CCCCC...CCCCC |   C =>|...|  * 127 => PAGE
o  ...  | ............. |       | 7 |
w PAGE6 | CCCCC...CCCCC |       +---+
s PAGE7 | CCCCC...CCCCC |         ^--- 8 1-bit pixels per column byte, arranged vertically regardless of addressing mode
        +---------------+ 
		 X => Pointer at PAGE 0, COL 0
		 
[!] Fuck yeah tile graphics	 
For the purposes of sd1306_move(), sd1306_home(), sd1306_puts(), s_putc() and most everything else,
the display is a 16x8 array of 8x8 character cells. Functions expect horizontal addressing mode, other modes
will make them act wanky. Pixels aren't really addressable; the tiny1634 doesn't have enough RAM for a local framebuffer,
and the SPI link to the display is one way so there's no read-modify-write using the display frame buffer.
Given the memory layout I'm reasonably certain the SSD1306 was intended to be driven, perhaps primarily, as a tile graphic display.

[!] Every (control byte 0x00 + command/parameter) word must followed by i2c stop.
So under linux, each word must be written seperately.
On the other hand, GDRAM data must be write with head 0x40 and data length not limited.
Under linux, every single data write must begin with head 0x40.

[!] Zero to Pixels
> Set pin/port defs in ssd1306-config.h
> sd1306_init()
> sd1306_clear() // buffer is full of entropy on boot
> sd1306_home()  // pointers should end up here, but let's make sure
> sd1306_puts("POOP") // POOP

*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "c64_lower.h"
#include "w2.h"

#define ADDRESS   0x3C	// 00111100  //+SA0+rw

#define COMCTRL 0x00
#define DATCTRL 0x40

#define SETCONTRAST 0x81
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYALLON 0xA5
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF

#define SETDISPLAYOFFSET 0xD3
#define SETCOMPINS 0xDA

#define SETVCOMDETECT 0xDB

#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE 0xD9

#define SETMULTIPLEX 0xA8

#define SETLOWCOLUMN 0x00
#define SETHIGHCOLUMN 0x10

#define SETSTARTLINE 0x40

#define MEMORYMODE 0x20
#define COLUMNADDRESS 0x21
#define PAGEADDRESS 0x22

#define COMSCANINC 0xC0
#define COMSCANDEC 0xC8

#define SEGREMAP 0xA0

#define CHARGEPUMP 0x8D

#define HSCROLLRIGHTSETUP 0x26
#define HSCROLLLEFTSETUP 0x27

#define HVSCROLLSETUP 0x29

#define ACTSCROLL 0x2F
#define DEACTSCROLL 0x2E

static uint8_t buf[9]; //is 9 enough for w2 send?
static char sbuf[17];
static __bit busy=0;

static PT(send_cmd_,uint8_t c){
	buf[0]=COMCTRL;
	buf[1]=c;
	PT_NEST_CALL(w2_master_write,ADDRESS,buf,2);
}

#define send_cmd(cmd) PT_CALL(send_cmd_,cmd)

PT_V(ssd1306_init){
/*	this won't work, as the chip wants a stop after every command word.
	uint8_t buf[]={DISPLAYOFF,SETDISPLAYCLOCKDIV,0x81,SETMULTIPLEX,0x3F,
		SETDISPLAYOFFSET,0x0,SETSTARTLINE | 0x0,CHARGEPUMP,0x14,
		MEMORYMODE,0x0,COLUMNADDRESS,0x0,0x7f,PAGEADDRESS,0x0,0x7,
		SEGREMAP | 0x1,COMSCANDEC,SETCOMPINS,0x12,SETCONTRAST,0x7f,
		SETPRECHARGE,0x11,SETVCOMDETECT,0x20,NORMALDISPLAY,
		DISPLAYALLON_RESUME,DISPLAYON};
	iic_write(fd,ADDRESS,buf,sizeof(buf));
*/
PT_B()
	PT_LOCK(busy);
	
    send_cmd(DISPLAYOFF);

    send_cmd(SETDISPLAYCLOCKDIV);
    send_cmd(0x80); //0x81

    send_cmd(SETMULTIPLEX);
    send_cmd(0x3F);
    
    send_cmd(SETDISPLAYOFFSET);
    send_cmd(0x00);
    
    send_cmd(SETSTARTLINE | 0x00);
    
    // We use internal charge pump
    send_cmd(CHARGEPUMP);
    send_cmd(0x14);
    
    // Horizontal memory mode
    send_cmd(MEMORYMODE);
    send_cmd(0x00);
    
    send_cmd(SEGREMAP | 0x01);

    send_cmd(COMSCANDEC);

    send_cmd(SETCOMPINS);
    send_cmd(0x12);

    // Max contrast
    send_cmd(SETCONTRAST);
    send_cmd(0x7F);

    send_cmd(SETPRECHARGE);
    send_cmd(0xF1); //0x11

    send_cmd(SETVCOMDETECT);
    send_cmd(0x40); //0x20

    send_cmd(DISPLAYALLON_RESUME);

    // Non-inverted display
    send_cmd(NORMALDISPLAY);

	send_cmd(DEACTSCROLL);

    // Turn display back on
    send_cmd(DISPLAYON);
    
    PT_UNLOCK(busy);
    
PT_E(outi)
}

PT_V(ssd1306_displayoff){
	PT_NEST_CALL(send_cmd_,DISPLAYOFF);
}

PT_V(ssd1306_displayon){
	PT_NEST_CALL(send_cmd_,DISPLAYON);
}

static PT(ssd1306_set_clock,uint8_t val) {
	PT_B()
	send_cmd(SETDISPLAYCLOCKDIV);
	send_cmd(val);
	PT_E(outclock)
}

static PT(ssd1306_set_addr_mode,uint8_t mode) {
	// 0 -> horizontal (write column, increment column pointer, at column overrun reset column pointer and increment page pointer)
	// 1 -> vertical (write column, increment page pointer, at page overrun reset page pointer and increment column pointer)
	// 2 -> page (write column, increment column pointer, reset column pointer at overrun)
	PT_B()
	send_cmd(MEMORYMODE);
	send_cmd((mode < 2) ? mode : 0);
	PT_E(outaddrm)	
}

PT(ssd1306_invert, uint8_t goinvert){
	PT_NEST_CALL(send_cmd_,goinvert?INVERTDISPLAY:NORMALDISPLAY);
}

PT(ssd1306_contrast,uint8_t contr){
	PT_B()
	send_cmd(SETCONTRAST);
	send_cmd(contr);
	PT_E(outctrst)
}

static PT(ssd1306_tile, uint8_t * tbuf, uint16_t size){ //tile must reserve [0] for head DATCTRL
	tbuf[0]=DATCTRL;
	PT_NEST_CALL(w2_master_write,ADDRESS,tbuf,size);
}

static PT(s_putc,char c) {
PT_B()
	// remap from petscii to ascii, shifts drawing characters into the lower 32 ascii cells
	if(c > 'A' && c < 'Z') { }               // upper-case ascii range
	else if(c >= 'a' && c <= 'z') { c -= 96; } // lower-case ascii range
	else if(c > 31 && c < 64) { }            // numbers and symbols
	else if(c < 32) { c += 96; }             // low ascii
	memcpy(buf+1,font + ((uint16_t)c<<3),8);
	PT_CALL(ssd1306_tile,buf,9);
PT_E(outputc)
}

static PT(ssd1306_range,uint8_t colstart, uint8_t colend, uint8_t pagestart, uint8_t pageend){
PT_B()
	send_cmd(COLUMNADDRESS);
	send_cmd(colstart);
	send_cmd(colend);
	send_cmd(PAGEADDRESS);
	send_cmd(pagestart);
	send_cmd(pageend);
PT_E(outrng)
}

static PT_V(ssd1306_home) {
	PT_NEST_CALL(ssd1306_range,0x0,0x7f,0x0,0x7);
}

static PT(ssd1306_move,uint8_t row, uint8_t col) {
	PT_NEST_CALL(ssd1306_range,col << 3, 0x7f,row,0x7);
}

PT_V(ssd1306_clear) {
	uint8_t i=0;
	PT_B()
	PT_LOCK(busy);
	PT_CALLV(ssd1306_home);
	memset(buf,0,9);
	for(; i < 128 ; ++i)PT_CALL(ssd1306_tile,buf,9);
	PT_UNLOCK(busy);
	PT_E(outclr)
}

PT(ssd1306_printf,uint8_t row, const char * format, ...){
	va_list args;
PT_B()
	PT_LOCK(busy);
	va_start(args,format);
	vsprintf(sbuf,format,args);
	va_end(args);
	sbuf[16]=0; //force trancation
	PT_CALL(ssd1306_move,row,0);
	while(*sbuf != 0) {
		PT_CALL(s_putc,*sbuf);
		memmove(sbuf,sbuf+1,sizeof(sbuf)-1);
	}
	PT_UNLOCK(busy);
PT_E(outprtf)
}

PT(ssd1306_hscroll,char goleft,uint8_t startpage, uint8_t endpage, 
	uint8_t startcol, uint8_t endcol, uint8_t speed){ //page 0~7; col virtual offset 0~0xff; speed 0~0xff
PT_B()
	PT_LOCK(busy);
	send_cmd(DEACTSCROLL);
	send_cmd(goleft?HSCROLLLEFTSETUP:HSCROLLRIGHTSETUP);
	send_cmd(0x0);
	send_cmd(startpage);
	send_cmd(speed);
	send_cmd(endpage);
	send_cmd(startcol);
	send_cmd(endcol); //speed?
	
	send_cmd(ACTSCROLL);
	PT_UNLOCK(busy);
PT_E(outhscrll)
}

PT_V(ssd1306_stopscroll){
	PT_NEST_CALL(send_cmd_,DEACTSCROLL);
}
