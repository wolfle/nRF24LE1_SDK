#ifndef SS1306_H_
#define SS1306_H_

#include <stdint.h>
#include <stdbool.h>
#include "pt.h"

void putchar(char c); //for printf_tiny

PT_V(ssd1306_init);//2
PT_V(ssd1306_displayoff);//2
PT_V(ssd1306_displayon);//2
//PT(ssd1306_invert, bool goinvert);//1
//PT(ssd1306_contrast, uint8_t contr);//2
PT_V(ssd1306_clear);//3
/*
PT(ssd1306_hscroll, __xdata char goleft,__xdata uint8_t startpage, __xdata uint8_t endpage, 
	__xdata uint8_t startcol, __xdata uint8_t endcol, __xdata uint8_t speed);//2
PT_V(ssd1306_stopscroll);//1
*/
PT(ssd1306_printf,uint8_t row, uint8_t col,__code const char * format, ...);//3

#endif
