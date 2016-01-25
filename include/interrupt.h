/////////////////////////////////////////////////////////////////////////////
//
// File: interrupt.h
//
// Copyright S. Brennen Ball, 2011
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

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

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "reg24le1.h"


///////////////////////////////////////////////////////////////////////////////
// Interrupt Vector Locations
///////////////////////////////////////////////////////////////////////////////
#define IRQ_V_IFP			0   //interrupt from pin
#define IRQ_V_T0				1   //timer0 overflow
#define IRQ_V_PWR_FAIL		2   //power failure
#define IRQ_V_T1				3   //timer1 overflow
#define IRQ_V_UART			4   //UART receive & transmit interrupt
#define IRQ_V_T2				5   //timer2 overflow
#define IRQ_V_RFRDY			8   //RF SPI ready interrupt
#define IRQ_V_RFIRQ			9   //RF interrupt
#define IRQ_V_SPI_2WIRE		10  //SPI/2-wire interrupt
#define IRQ_V_WU_ON_PIN		11  //wakeup on pin interrupt
#define IRQ_V_XOSC_ADC_RNG	12  //XOSC/ADC/RNG interrupt
#define IRQ_V_RTC2			13  //wakeup from RTC2


///////////////////////////////////////////////////////////////////////////////
// Interrupt enable/disable macros
///////////////////////////////////////////////////////////////////////////////
#define irq_global_enable()			sbit_set(IEN0_SB_GLOBAL)		//Enables the global interrupt control bit
#define irq_global_disable()			sbit_clear(IEN0_SB_GLOBAL)		//Disables the global interrupt control bit
#define irq_ifp_enable()				sbit_set(IEN0_SB_IFP)			//Enables the interrupt from pin control bit
#define irq_ifp_disable()				sbit_clear(IEN0_SB_IFP)			//Disables the interrupt from pin control bit
#define irq_t0_enable()				sbit_set(IEN0_SB_T0)			//Enables the timer 0 interrupt control bit
#define irq_t0_disable()				sbit_clear(IEN0_SB_T0)			//Disables the timer 0 interrupt control bit
#define irq_pwr_fail_enable()			sbit_set(IEN0_SB_POFIRQ)		//Enables the power failure interrupt control bit
#define irq_pwr_fail_disable()		sbit_clear(IEN0_SB_POFIRQ)		//Disables the power failure interrupt control bit
#define irq_t1_enable()				sbit_set(IEN0_SB_T1)			//Enables the timer 1 interrupt control bit
#define irq_t1_disable()				sbit_clear(IEN0_SB_T1)			//Disables the timer 1 interrupt control bit
#define irq_uart_enable()				sbit_set(IEN0_SB_UART)			//Enables the UART interrupt control bit
#define irq_uart_disable()			sbit_clear(IEN0_SB_UART)		//Enables the UART interrupt control bit
#define irq_t2_enable()				sbit_set(IEN0_SB_T2)			//Enables the timer 2 interrupt control bit
#define irq_t2_disable()				sbit_clear(IEN0_SB_T2)			//Disables the timer 2 interrupt control bit
#define irq_rfrdy_enable()			sbit_set(IEN1_SB_RFRDY)			//Enables the RF SPI ready interrupt control bit
#define irq_rfrdy_disable()			sbit_clear(IEN1_SB_RFRDY)		//Disables the RF SPI ready interrupt control bit
#define irq_rfirq_enable()			sbit_set(IEN1_SB_RFIRQ)			//Enables the RF communication interrupt control bit
#define irq_rfirq_disable()			sbit_clear(IEN1_SB_RFIRQ)		//Disables the RF communication interrupt control bit
#define irq_spi_2wire_enable()		sbit_set(IEN1_SB_SPI_2WIRE)		//Enables the SPI/2 wire interrupt control bit
#define irq_spi_2wire_disable()		sbit_clear(IEN1_SB_SPI_2WIRE)	//Disables the SPI/2 wire interrupt control bit
#define irq_wu_on_pin_enable()		sbit_set(IEN1_SB_WUOPIRQ)		//Enables the wake-up on pin interrupt control bit
#define irq_wu_on_pin_disable()		sbit_clear(IEN1_SB_WUOPIRQ)		//Disables the wake-up on pin interrupt control bit
#define irq_xosc_adc_rng_enable()		sbit_set(IEN1_SB_MISCIRQ)		//Enables the X0SC16M started/ADC ready/RNG ready interrupt control bit
#define irq_xosc_adc_rng_disable()	sbit_clear(IEN1_SB_MISCIRQ)		//Disables the X0SC16M started/ADC ready/RNG ready interrupt control bit
#define irq_rtc2_enable()				sbit_set(IEN1_SB_TICK)			//Enables the internal wakeup from RTC2 interrupt control bit
#define irq_rtc2_disable()			sbit_clear(IEN1_SB_TICK)		//Disables the internal wakeup from RTC2 interrupt control bit


///////////////////////////////////////////////////////////////////////////////////
// Macros to save and restore the global interrupt flag into a user-specified sbit
///////////////////////////////////////////////////////////////////////////////////
#define irq_save_global_flag(local_sbit)		local_sbit = IEN0_SB_GLOBAL
#define irq_restore_global_flag(local_sbit)		IEN0_SB_GLOBAL = local_sbit

#define irq_save_rf_flag(local_sbit)	local_sbit = IEN1_SB_RFIRQ
#define irq_restore_rf_flag(local_sbit)	 IEN1_SB_RFIRQ = local_sbit


///////////////////////////////////////////////////////////////////////////////
// cli()/sti() instructions
///////////////////////////////////////////////////////////////////////////////
#define cli()	irq_global_disable()
#define sti()	irq_global_enable()
#define cli_save(lsbit) {irq_save_global_flag(lsbit);cli();}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt function headers, which can be used in both prototypes and definitions (these do not incorporate __using(<bank>) directives)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define irq_isr_ifp()				void isr_ifp()				__interrupt(IRQ_V_IFP)			//Interrupt from pin interrupt function header
#define irq_isr_t0()				void isr_t0()				__interrupt(IRQ_V_T0)			//Timer 0 interrupt function header
#define irq_isr_pwr_fail()		void isr_pwr_fail()			__interrupt(IRQ_V_PWR_FAIL)		//Power failure interrupt function header
#define irq_isr_t1()				void isr_t1() 				__interrupt(IRQ_V_T1)			//Timer 1 interrupt function header
#define irq_isr_uart()			void isr_uart()				__interrupt(IRQ_V_UART)			//UART interrupt function header
#define irq_isr_t2()				void isr_t2()				__interrupt(IRQ_V_T2)			//Timer 2 interrupt function header
#define irq_isr_rfrdy()			void isr_rfrdy()			__interrupt(IRQ_V_RFRDY)			//RF SPI ready interrupt function header
#define irq_isr_rfirq()			void isr_rfirq()			__interrupt(IRQ_V_RFIRQ)			//RF communication interrupt function header
#define irq_isr_spi_2wire()		void isr_spi()				__interrupt(IRQ_V_SPI_2WIRE)		//SPI/2 wire interrupt function header
#define irq_isr_wu_on_pin()		void isr_wu_on_pin()		__interrupt(IRQ_V_WU_ON_PIN)		//Wake-up on pin interrupt function header
#define irq_isr_xosc_adc_rng()	void isr_xosc_adc_rng()		__interrupt(IRQ_V_XOSC_ADC_RNG)	//X0SC16M started/ADC ready/RNG ready interrupt function header
#define irq_isr_rtc2()			void isr_rtc2()				__interrupt(IRQ_V_RTC2)			//Internal wakeup from RTC2 interrupt function header


///////////////////////////////////////////////////////////////////////////////
// Macros to check interrupt flags
///////////////////////////////////////////////////////////////////////////////
#define irq_flag_uart_tx		(S0CON_SB_TI0)			
#define irq_flag_uart_rx		(S0CON_SB_RI0)			
#define irq_flag_rfrdy		(IRCON_SB_RFRDY)		
#define irq_flag_rfirq		(IRCON_SB_RFIRQ)		
#define irq_flag_spi_2wire	(IRCON_SB_SPI_2WIRE)	
#define irq_flag_wuopirq		(IRCON_SB_WUOPIRQ)		
#define irq_flag_miscirq		(IRCON_SB_MISCIRQ)		
#define irq_flag_tick			(IRCON_SB_TICK)			
#define irq_flag_tf2			(IRCON_SB_TF2)			
#define irq_flag_exf2			(IRCON_SB_EXF2)			
#define irq_flag_ifp0			(TCON_SB_IE0)
#define irq_flag_ifp1			(TCON_SB_IE1)
#define irq_flag_ifp2			(?)				//FIXME

//clear/set irq flag
#define __IDENT_(x) x
#define __STRIP(x) __IDENT_ x 	// to strip the outermost () of x
#define irq_flag_clear(flag)		sbit_clear(__STRIP(flag))
#define irq_flag_set(flag)		sbit_set(__STRIP(flag))

//wait for irq flag and clear it
#define irq_wait_flag(flag)	do{while(!flag);irq_flag_clear(flag);}while(0)

///////////////////////////////////////////////////////////////////////////////
// Macros to unmask interrupts in the INTEXP register
///////////////////////////////////////////////////////////////////////////////
#define irq_unmask_sspi()		reg_bits_set(INTEXP, INTEXP_SSPI_COMPLETED_INT_ENABLE)		//Unmasks SSPI completed interrupt in INTEXP
#define irq_mask_sspi()			reg_bits_clear(INTEXP, INTEXP_SSPI_COMPLETED_INT_ENABLE)	//Masks SSPI completed interrupt in INTEXP
#define irq_unmask_mspi()		reg_bits_set(INTEXP, INTEXP_MSPI_COMPLETED_INT_ENABLE)		//Unmasks MSPI completed interrupt in INTEXP
#define irq_mask_mspi()			reg_bits_clear(INTEXP, INTEXP_MSPI_COMPLETED_INT_ENABLE)	//Masks MSPI completed interrupt in INTEXP
#define irq_unmask_2wire()		reg_bits_set(INTEXP, INTEXP_2WIRE_COMPLETED_INT_ENABLE)		//Unmasks MSPI completed interrupt in INTEXP
#define irq_mask_2wire()		reg_bits_clear(INTEXP, INTEXP_2WIRE_COMPLETED_INT_ENABLE)	//Masks MSPI completed interrupt in INTEXP
#define irq_unmask_gpint_ifp(gpint_sel)	reg_bits_set(INTEXP, gpint_sel)								//Unmasks specified GPINTX interrupt from pin(s) in INTEXP
#define irq_mask_gpint_ifp(gpint_sel)		reg_bits_clear(INTEXP, gpint_sel)							//Masks specified GPINTX interrupt from pin(s) in INTEXP
#define irq_unmask_gpint0_ifp()			irq_unmask_gpint_ifp(INTEXP_GP_INT0_ENABLE)			//Unmasks GPINT0 interrupt from pin in INTEXP
#define irq_mask_gpint0_ifp()				irq_mask_gpint_ifp(INTEXP_GP_INT0_ENABLE)				//Masks GPINT0 interrupt from pin in INTEXP
#define irq_unmask_gpint1_ifp()			irq_unmask_gpint_ifp(INTEXP_GP_INT1_ENABLE)			//Unmasks GPINT1 interrupt from pin in INTEXP
#define irq_mask_gpint1_ifp()				irq_mask_gpint_ifp(INTEXP_GP_INT1_ENABLE)				//Masks GPINT1 interrupt from pin in INTEXP
#define irq_unmask_gpint2_ifp()			irq_unmask_gpint_ifp(INTEXP_GP_INT2_ENABLE)			//Unmasks GPINT2 interrupt from pin in INTEXP
#define irq_mask_gpint2_ifp()				irq_mask_gpint_ifp(INTEXP_GP_INT2_ENABLE)				//Masks GPINT2 interrupt from pin in INTEXP


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enumeration to hold indices for GPINTx enables for use in irq_configure_ifp()...use only one as irq_if_input argument
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	ifp_int0 = INTEXP_GP_INT0_ENABLE, //P0.0 on 24-pin, P0.5 on 32-pin, P1.2 on 48-pin
	ifp_int1 = INTEXP_GP_INT1_ENABLE,	//P0.2 on 24-pin, P0.6 on 32-pin, P1.3 on 48-pin
	ifp_int2 = INTEXP_GP_INT2_ENABLE	//Not present on 24-pin or 32-pin, P1.4 on 48-pin
} irq_ifp_input_e;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Enumeration to hold indices for interrupt priority groups for use in irq_set_priority()
///////////////////////////////////is_flag_active//////////////////////////////////////////////////////////////
typedef enum
{
	p_ifp_rfrdy				= 0,
	p_tf0_rfirq				= 1,
	p_pofirq_msdone_ssdone	= 2,
	p_rf1_wuopirq			= 3,
	p_ri0_ti0_miscirq		= 4,
	p_tf2_exf2_tick			= 5
} irq_priority_group_e;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Enumeration to hold indices for interrupt priority levels for use in irq_set_priority()
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	irqp_0		= 0,	//Lowest priority
	irqp_1		= 1,
	irqp_2		= 2,
	irqp_3		= 3	//Highest priority
} irq_priority_e;


///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////
void irq_config_ifp(irq_ifp_input_e irq_ifp_input, bool enable, bool edge);
void irq_set_priority(irq_priority_group_e group, irq_priority_e level);

#endif /*INTERRUPT_H_*/
