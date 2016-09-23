#include "timer.h"
#include "pwr_clk_mgmt.h"
#include "interrupt.h"
#include "error.h"

#define TIMER0_TMOD_MASK	0x0F	//Mask for the options used for TMOD
#define TIMER1_TMOD_MASK	0xF0	//Mask for the options used for TMOD
#define TIMER2_CCEN_WRITE_MASK		0xFF	//Mask for the options used for CCEN
#define TIMER2_T2CON_WRITE_MASK		0x9B	//Mask for the options used for T2CON
#define RTC2_RTC2CON_MASK	0x0F	//Mask for the options used for RTC2CON

#define TIMER0_MODE_0_13_BIT_CTR_TMR			(0x00)	//Configure timer 0 in mode 0 (13-bit counter/timer)
#define TIMER0_MODE_1_16_BIT_CTR_TMR			(0x01 << TMOD_MODE0_SHIFT)	//Configure timer 0 in mode 1 (16-bit counter/timer)
#define TIMER0_MODE_2_8_BIT_AUTO_RLD_TMR		(0x02 << TMOD_MODE0_SHIFT)	//Configure timer 0 in mode 2 (8-bit timer with auto-reload)
#define TIMER0_MODE_3_TWO_8_BIT_CTRS_TMRS		(0x03 << TMOD_MODE0_SHIFT)	//Configure timer 0 in mode 3 (two 8-bit counters/timers)

#define TIMER0_FUNCTION_TIMER					(0)							//Configure timer 0 as a timer
#define TIMER0_FUNCTION_COUNT_EVENTS_ON_T0	(TMOD_CT0)					//Configure timer 0 to count events on T0

#define TIMER0_GATE_ALWAYS_RUN_TIMER			(0)							//Configure timer 0 to always run the timer
#define TIMER0_GATE_RUN_TIMER_WHEN_IFP_HIGH	(TMOD_GATE0)				//Configure timer 0 to run when IFP is high


#define TIMER1_MODE_0_13_BIT_CTR_TMR			(0x00)	//Configure timer 1 in mode 0 (13-bit counter/timer)
#define TIMER1_MODE_1_16_BIT_CTR_TMR			(0x01 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 1 (16-bit counter/timer)
#define TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR		(0x02 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 2 (8-bit timer with auto-reload)
#define TIMER1_MODE_3_TWO_8_BIT_CTRS_TMRS		(0x03 << TMOD_MODE1_SHIFT)	//Configure timer 1 in mode 3 (two 8-bit counters/timers)

#define TIMER1_FUNCTION_TIMER					(0)							//Configure timer 1 as a timer
#define TIMER1_FUNCTION_COUNT_EVENTS_ON_T1	(TMOD_CT1)					//Configure timer 1 to count events on T1

#define TIMER1_GATE_ALWAYS_RUN_TIMER			(0)							//Configure timer 1 to always run the timer
#define TIMER1_GATE_RUN_TIMER_WHEN_IFP_HIGH	(TMOD_GATE1)				//Configure timer 1 to run when IFP is high


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bit shifts used due to the fact that we're using one argument to hold values for 2 different hardware registers use
//  in timer2_configure_<x>() functions...DO NOT USE THESE DIRECTLY IN USER CODE!!!!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	TIMER2_CCEN_OFFSET_SHIFT	(8)		//Shift for the options used for CCEN...DO NOT USE IN CODE


#define TIMER2_MODE_STOPPED						(0x00)	//Stop the timer (no input)
#define TIMER2_MODE_TIMER							(0x01 )	//Set the input to f/12 or f/24 (set by option TIMER2_PRESCALER_DIV_*)
#define TIMER2_MODE_EVENT_COUNTER					(0x02 )	//Set the input to the falling edge of T2
#define TIMER2_MODE_GATED_TIMER					(0x03 )	//Set the input to f/12 or f/24 gated by T2

#define TIMER2_RELOAD_DISABLED					(0x00)	//Do not automatically reload the timer
#define TIMER2_RELOAD_ON_OVERFLOW				(0x02 << T2CON_RELOAD_MODE_SHIFT)	//Reload the timer on an overflow
#define TIMER2_RELOAD_ON_T2EX_FALL				(0x03 << T2CON_RELOAD_MODE_SHIFT)	//Reload the timer on a falling edge of T2EX (frequency of CLKLF/2)

#define TIMER2_PRESCALER_DIV_SHIFT				(7)									//Shift for prescaler option below (do not use this option)
#define TIMER2_PRESCALER_DIV_MASK				((1 << TIMER2_PRESCALER_DIV_SHIFT)<<TIMER2_TCON2_OFFSET_SHIFT)									//Mask for prescaler option below (do not use this option)
#define TIMER2_PRESCALER_DIV_12					(0)									//Set prescaler to divide CCLK by 12
#define TIMER2_PRESCALER_DIV_24					(1 << TIMER2_PRESCALER_DIV_SHIFT)									//Set prescaler to divide CCLK by 24

#define TIMER2_CMP_CAPT_CRC_DISABLED				(0x00)		//Disable compare/capture for CRC register
#define TIMER2_CMP_CAPT_CRC_CAPT_ON_WRITE_TO_CRCL	((0x03 ) << TIMER2_CCEN_OFFSET_SHIFT)		//Enable capturing the value of TH2 into CRC when a write is done to CRCL

#define TIMER2_CMP_CAPT_CC1_DISABLED				(0x00)		//Disable compare/capture for CC1 register
#define TIMER2_CMP_CAPT_CC1_CAPT_ON_WRITE_TO_CCL1	((0x03 << CCEN_CC1_MODE_SHIFT) << TIMER2_CCEN_OFFSET_SHIFT)		//Enable capturing the value of TH2 into CC1 when a write is done to CCL1

#define TIMER2_CMP_CAPT_CC2_DISABLED				(0x00)		//Disable compare/capture for CC2 register
#define TIMER2_CMP_CAPT_CC2_CAPT_ON_WRITE_TO_CCL2	((0x03 << CCEN_CC2_MODE_SHIFT) << TIMER2_CCEN_OFFSET_SHIFT)		//Enable capturing the value of TH2 into CC1 when a write is done to CCL2

#define TIMER2_CMP_CAPT_CC3_DISABLED				(0x00)		//Disable compare/capture for CC3 register
#define TIMER2_CMP_CAPT_CC3_CAPT_ON_WRITE_TO_CCL3	((0x03 << CCEN_CC3_MODE_SHIFT) << TIMER2_CCEN_OFFSET_SHIFT)		//Enable capturing the value of TH2 into CC1 when a write is done to CCL3

#define RTC2_DISABLE						(0)										//Don't start RTC2
#define RTC2_ENABLE						(RTC2CON_ENABLE)						//Start RTC2

#define RTC2_COMPARE_MODE_DISABLED		(0x00)	//Configure RTC2 compare in mode 0 (compare disabled)
#define RTC2_COMPARE_MODE_1_NO_RESET	(0x02 << RTC2CON_COMPARE_MODE_SHIFT)	//Configure RTC2 compare in mode 2 (wrap at IRQ)
#define RTC2_COMPARE_MODE_0_RESET_AT_IRQ	(0x03 << RTC2CON_COMPARE_MODE_SHIFT)	//Configure RTC2 compare in mode 3 (reset at IRQ)

#define RTC2_DO_NOT_CAPTURE_ON_RFIRQ		(0)										//Don't capture RTC2 registers on RFIRQ
#define RTC2_CAPTURE_ON_RFIRQ				(RTC2CON_ENABLE_EXTERNAL_CAPTURE)		//Capture RTC2 registers on RFIRQ


////////////////////////////
// Function macros
////////////////////////////
#define timer0_run()					sbit_set(TCON_SB_TR0)		//Run timer 0
#define timer0_stop()					sbit_clear(TCON_SB_TR0)		//Stop timer 0
#define timer0_is_running()				(TCON_SB_TR0)				//True if timer 0 is running, false otherwise
#define timer0_has_overflown()			(TCON_SB_TF0)				//True if timer 0 has overflown, false otherwise
#define timer0_clear_overflow_flag()	sbit_clear(TCON_SB_TF0)		//Clear timer 0 overflow flag (auto-cleared if using an IRQ)
#define timer0_set_t0_val(period)		T0 = period					//Update the value of TH0:TL0

#define timer1_run()					sbit_set(TCON_SB_TR1)		//Run timer 1
#define timer1_stop()					sbit_clear(TCON_SB_TR1)		//Stop timer 1
#define timer1_is_running()				(TCON_SB_TR1)				//True if timer 1 is running, false otherwise
#define timer1_has_overflown()			(TCON_SB_TF1)				//True if timer 1 has overflown, false otherwise
#define timer1_clear_overflow_flag()	sbit_clear(TCON_SB_TF1)		//Clear timer 1 overflow flag (auto-cleared if using an IRQ)
#define timer1_set_t1_val(period)		T1 = period					//Update the value of TH1:TL1

#define timer2_run_as_timer()				sbit_clear(T2CON_SB_T2I1); sbit_set(T2CON_SB_T2I0)										//Run timer 2 in timer mode
#define timer2_run_as_event_counter()		sbit_set(T2CON_SB_T2I1); sbit_clear(T2CON_SB_T2I0)										//Run timer 2 in event counter mode
#define timer2_run_as_gated_timer()			sbit_set(T2CON_SB_T2I1); sbit_set(T2CON_SB_T2I0)										//Run timer 2 in gated timer mode
#define timer2_stop()						sbit_clear(T2CON_SB_T2I1); sbit_clear(T2CON_SB_T2I0)									//Stop timer 2
#define timer2_is_running()					(T2CON_SB_T2I0 || T2CON_SB_T2I1)														//True if timer 2 is running, false otherwise
#define timer2_set_crc_val(period)			CRC = period																			//Update the value of CRCH:CRCL
#define timer2_set_t2_val(period)			T2 = period																				//Update the value of TH2:TL2
#define timer2_set_crc_and_t2_vals(period)	timer2_set_crc_val(period); timer2_set_t2_val(period)									//Update both CRC and T2 with the same value

#define rtc2_run()			reg_bits_set(RTC2CON, RTC2CON_ENABLE) 			//Run RTC2
#define rtc2_stop()			reg_bits_clear(RTC2CON, RTC2CON_ENABLE)			//Stop RTC2
#define rtc2_is_running()	(RTC2CON & RTC2CON_ENABLE)		//True if RTC2 is running, false otherwise
#define rtc2_sfr_capture()	reg_bits_set(RTC2CON, RTC2CON_SFR_CAPTURE) 		//Captures RTC2 registers

static 	timer_callback_t cb1,cb2,rtcb;
static  volatile  uint16_t dms;  //*0.1ms, used as jiffers

uint32_t timer_c_to_us(uint16_t c)
{
	//Calculates the desired timer overflow period from the CRC value
	//
	//          (65536 - CRC) * (2 ^ PRESCALER) * 12 clocks/timer++
	// period = ---------------------------------------------------
	//                               CCLK_FREQ
	return (uint32_t)c * 12 / get_cclk_freq_in_mhz();
}

uint32_t timer_us_to_c(uint32_t us)
{
	//Calculates the CRCH:CRCL value from the desired timer overflow period
	//
	//                      period * CCLK_FREQ
	//CRC = 65536 - -----------------------------------
	//              (2 ^ PRESCALER) * 12 clocks/timer++
	//
	//Round the floating point number to the nearest integer by adding 0.5, then return that integer
	return us * get_cclk_freq_in_mhz() / 12 ;
}

void timer0_init(uint8_t timer0_config_options, uint16_t t0_val)
{
	timer0_stop();
	irq_t0_disable();
	//If timer 0 is being run as 2 8-bit timers with auto-reload, then set the reload value in TH0 and TL0
	if((timer0_config_options & TMOD_MODE0_MASK) == TIMER0_MODE_2_8_BIT_AUTO_RLD_TMR){
		TH0 = (uint8_t)t0_val;
		TL0 = (uint8_t)t0_val;
	}else{
		timer0_set_t0_val(t0_val);
	}

	//Set up TMOD register from timer0_config_options
	TMOD = (TMOD & ~TIMER0_TMOD_MASK) | (timer0_config_options & TIMER0_TMOD_MASK);
	irq_t0_enable();
	timer0_run();
}

void timer1_init(uint8_t timer1_config_options, uint16_t t1_val, timer_callback_t callback)
{
	timer1_stop();
	irq_t1_disable();
	//If timer 1 is being run as 2 8-bit timers with auto-reload, then set the reload value in TH1 and TL1
	if((timer1_config_options & TMOD_MODE1_MASK) == TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR){
		TH1 = (uint8_t)t1_val;
		TL1 = (uint8_t)t1_val;
	}else{
		timer1_set_t1_val(t1_val);
	}

	//Set up TMOD register from timer1_config_options
	TMOD = (TMOD & ~TIMER1_TMOD_MASK) | (timer1_config_options & TIMER1_TMOD_MASK);
	cb1=callback;
	irq_t1_enable();
	timer1_run();
}

//  uint16_t auto_reload_or_compare_value - 
//		if using auto-reload (either mode), 
//					this is the auto-reload value used to init CRC ***and*** T2
//      if not using auto-reload, but instead using compare with CCEN,
//					this is the compare value used to init CRC (T2 is init to 0)
void timer2_init(uint16_t timer2_config_options, uint16_t auto_reload_or_compare, timer_callback_t callback)
{
	timer2_stop();
	irq_t2_disable();
	//If timer 2 is being run in either auto-reload mode, then set the reload value in T2 and CRC
	if(timer2_config_options & T2CON_RELOAD_MODE_MASK ){
		timer2_set_crc_and_t2_vals(auto_reload_or_compare);
	}else{ 
		timer2_set_crc_val(auto_reload_or_compare);
	}
	//Set up CCEN and T2CON registers from timer2_config_options
	CCEN = ((uint8_t)(timer2_config_options >> TIMER2_CCEN_OFFSET_SHIFT)) & TIMER2_CCEN_WRITE_MASK;
	T2CON = (T2CON & ~TIMER2_T2CON_WRITE_MASK) | (timer2_config_options&TIMER2_T2CON_WRITE_MASK);
	cb2=callback;
	irq_t2_enable();
}

void rtc2_init(uint8_t rtc2_config_options, uint16_t compare_value, timer_callback_t callback)
{
	rtc2_stop();
	irq_rtc2_disable();
	//If RTC2 is running in either mode 0 or 1, then setup the compare value
	if(rtc2_config_options & RTC2CON_COMPARE_MODE_MASK)rtc2_set_compare(compare_value);

	//Set up RTC2CON register from rtc2_config_options
	RTC2CON = (RTC2CON & ~RTC2_RTC2CON_MASK) | (rtc2_config_options & RTC2_RTC2CON_MASK);
	rtcb=callback;
	irq_rtc2_enable();
	rtc2_run();
}

void dms_init(void){
	dms=0;
	timer0_init(TIMER0_MODE_2_8_BIT_AUTO_RLD_TMR,0xFF-133+1);
}

uint16_t get_dms(void){
	return dms;
}

irq_isr_t0(){
	++dms;
//	if(!TMOD&TIMER0_MODE_2_8_BIT_AUTO_RLD_TMR)timer0_stop();
}

irq_isr_t1(){
	if(cb1)cb1();
	if(!TMOD&TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR)timer1_stop();	
}

irq_isr_t2(){
	if(cb2)cb2();
	if(!T2CON&T2CON_RELOAD_MODE_MASK)timer2_stop();	
}

irq_isr_rtc2(){
	if(rtcb)rtcb();
}

char timer_once(uint32_t us, timer_callback_t callback)
{
	uint32_t c=timer_us_to_c(us);
	if(c>0x1FFFF)return -EINVAL;
	if(c>0xFFFF){
		if(cb2)return -EBUSY;
		timer2_init(TIMER2_MODE_TIMER|TIMER2_PRESCALER_DIV_24,c>>1,callback);
	}else{
		if(!cb1)timer1_init(TIMER1_MODE_1_16_BIT_CTR_TMR,0xFFFF-c+1,callback);
		else if(!cb2)timer2_init(TIMER2_MODE_TIMER|TIMER2_PRESCALER_DIV_12,c,callback);		
		else return -EBUSY;
	}
	return SUCCESS;
}

char timer_repeat(uint32_t us, timer_callback_t callback)
{
	uint32_t c=timer_us_to_c(us);
	if(c>0x1FFFF)return -EINVAL;
	if(c>0xFFFF){
		if(cb2)return -EBUSY;
		timer2_init(TIMER2_MODE_TIMER|TIMER2_PRESCALER_DIV_24|TIMER2_RELOAD_ON_OVERFLOW,0xFFFF-(c>>1)+1,callback);
	}else if(c<=0xFF){
		if(!cb1)timer1_init(TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR,0xFF-c+1,callback);
		else if(!cb2)timer2_init(TIMER2_MODE_TIMER|TIMER2_RELOAD_ON_OVERFLOW,0xFF-c+1,callback);
		else return -EBUSY;
	}else{
		if(cb2)return -EBUSY;
		timer2_init(TIMER2_MODE_TIMER|TIMER2_RELOAD_ON_OVERFLOW,0xFFFF-c+1,callback);
	}		
	return SUCCESS;
}

PT(delay_dms,uint16_t sms,uint16_t * j0, uint16_t *j1){
	PT_B()
	*j0=dms;
	*j1=*j0+sms;
	if(*j1<*j0)PT_WAIT(dms<*j0);
	PT_WAIT(dms>*j1);
	PT_E(outdelay)
}
