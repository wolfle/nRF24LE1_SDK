/////////////////////////////////////////////////////////////////////////////
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////

#ifndef _TIMER_H_
#define _TIMER_H_

#include "reg24le1.h"
#include "pt.h"

#define TIMER1_MODE_0_13_BIT_CTR_TMR			(0x00)	//Configure timer 1 in mode 0 (13-bit counter/timer)
#define TIMER1_MODE_1_16_BIT_CTR_TMR			(0x01 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 1 (16-bit counter/timer)
#define TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR		(0x02 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 2 (8-bit timer with auto-reload)
#define TIMER1_MODE_3_TWO_8_BIT_CTRS_TMRS		(0x03 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 3 (two 8-bit counters/timers)

#define TIMER1_FUNCTION_TIMER					(0)							//Configure timer 1 as a timer
#define TIMER1_FUNCTION_COUNT_EVENTS_ON_T1	(TMOD_CT1)					//Configure timer 1 to count events on T1

#define TIMER1_GATE_ALWAYS_RUN_TIMER			(0)							//Configure timer 1 to always run the timer
#define TIMER1_GATE_RUN_TIMER_WHEN_IFP_HIGH	(TMOD_GATE1)				//Configure timer 1 to run when IFP is high

//typedef void (*timer_callback_t)(void) __reentrant;

////////////////////////////
// Function prototypes
////////////////////////////
//void timer0_init(uint8_t timer0_config_options, uint16_t t0_val);

void timer1_init(uint8_t timer1_config_options,uint16_t t1_val) __reentrant;

void timer2_init(uint16_t timer2_config_options,uint16_t auto_reload_or_compare_time) __reentrant;
//uint32_t timer_us_to_c(uint32_t us);
//uint32_t timer_c_to_us(uint16_t c);

void rtc2_init(uint8_t rtc2_config_options, uint16_t compare_value) __reentrant;
static inline void rtc2_set_compare(uint16_t compare_value)
{
	RTC2CMP0 = (uint8_t)compare_value;
	RTC2CMP1 = (uint8_t)(compare_value >> 8);
}

void dms_init(void);
//uint16_t get_dms(void);

//char timer_once(uint32_t us, timer_callback_t callback);
//char timer_repeat(uint32_t us, timer_callback_t callback);
PT(delay_dms, uint16_t sms,__pdata uint16_t * j0, __pdata uint16_t * j1); //1
void delay_us(uint8_t microseconds);

#endif /* _TIMER_H_ */
