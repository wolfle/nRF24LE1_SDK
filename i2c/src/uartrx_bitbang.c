#include "timer.h"
#include "gpio.h"
#include "interrupt.h"
#include "uartrx_bitbang.h"

#define PIN 	GPIO_PIN_ID_FUNC_GPINT1     // which pin for serial input 
#define SER_BAUD 555             //  (16Mhz/12/2400 baud) = 555.6

static unsigned char data;            // holds the serial byte that was received
static unsigned char i;
static uartcb_t uartcb=0;


//timer interrupt: get pin value into temp byte, reset timer; if its stop bit, enable efp
irq_isr_t1(){
	if(i){
		//reload timer
		timer1_init(TIMER1_MODE_1_16_BIT_CTR_TMR,0xFFFF-SER_BAUD+1);

		data >>=1; //lsb first
		if(!gpio_read(PIN))data |=0x80; //invert input
		--i;
		if(!i&&uartcb)uartcb(data); //call the callback if whole byte have received
	}else{
		//enable ifp
		irq_ifp_enable();
	}
}

//efp interrupt: start bit begin, disable efp int, set timer to delay 1.5 baud
irq_isr_ifp(){
	irq_ifp_disable();
	timer1_init(TIMER1_MODE_1_16_BIT_CTR_TMR,0xFFFF-(SER_BAUD+(SER_BAUD>>1))+1);
	i=8;
}

void uartrx_init(uartcb_t cb){  //pin, baud?
	uartcb=cb;
	irq_config_ifp(ifp_int1,true,true);
}
