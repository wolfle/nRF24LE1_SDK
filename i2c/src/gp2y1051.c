#include "gp2y1051.h"
#include "uartrx_bitbang.h"

//7bytes: 0xaa vh vl vrefh vrefl crc 0xff
//we only need the vh and vl, do not care about crc and vref

static uint16_t v;
static uint8_t i=0;
static gp2ycb_t cb=0;

static void uartcb(unsigned char d) __reentrant{
	switch(i){
		case 0:	//0xaa
			if(d==0xaa)++i;
			break;
		case 1:	//vh
			v=(d<<8);
			++i;
			break;
		case 2:	//vl
			v+=d;
			++i;
			break;
		case 3: //vrefh
		case 4:	//vrefl
		case 5:	//crc
			++i;
			break;
		case 6: //0xff
			if(d==0xff){
				i=0;
				//call back to report new v
				if(cb)cb(v);
			}else ++i;
			break;
		default: //i==7; misaligned, wait for 0xff to sync
			if(d==0xff)i=0;
			break;
	}
}

void gp2y_init(gp2ycb_t cb_){
	cb=cb_;
	uartrx_init(uartcb);
}
