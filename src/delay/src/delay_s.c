#include "delay.h"

void delay_s(uint16_t seconds)
{
	while(seconds--)
	{
		delay_ms(1000);
	}
}

