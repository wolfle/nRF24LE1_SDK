#ifndef SS1306_H_
#define SS1306_H_

#include <stdint.h>
#include "pt.h"

PT_V(ssd1306_init);
PT_V(ssd1306_displayoff);
PT_V(ssd1306_displayon);
PT(ssd1306_invert, uint8_t goinvert);
PT(ssd1306_contrast,uint8_t contr);
PT_V(ssd1306_clear);

PT(ssd1306_hscroll, char goleft,uint8_t startpage, uint8_t endpage, 
	uint8_t startcol, uint8_t endcol, uint8_t speed);
PT_V(ssd1306_stopscroll);

PT(ssd1306_printf,uint8_t row, const char * format, ...);

#endif
