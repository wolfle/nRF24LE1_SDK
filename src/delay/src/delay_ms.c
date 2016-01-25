#include "delay.h"

void delay_ms(uint16_t milliseconds)
{
	while(milliseconds--){
		delay_us(1000);
	}
}

