/////////////////////////////////////////////////////////////////////////////
//
// File: pwr_clk_mgmt.h
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

#ifndef PWR_CLK_MGMT_H_
#define PWR_CLK_MGMT_H_

#include "reg24le1.h"

///////////////////////////////////////////
// Compilation defines
///////////////////////////////////////////
// Verify existence of __TARG_PACKAGE_TYPE
#ifndef __TARG_PACKAGE_TYPE
#error "You must define __TARG_PACKAGE_TYPE in the appropriate target_nrf24le1_sdk.h file to use this header file"
#endif

// Verify that __TARG_PACKAGE_TYPE has been set to an appropriate value
#if (__TARG_PACKAGE_TYPE != NRF24LE1_PACKAGE_24_PIN) && (__TARG_PACKAGE_TYPE != NRF24LE1_PACKAGE_32_PIN) && (__TARG_PACKAGE_TYPE != NRF24LE1_PACKAGE_48_PIN)
#error "__TARG_PACKAGE_TYPE has been defined, but it has been defined to an incorrect value.  Please see #defines NR24LE1_PACKAGE_XX_PIN in include/reg24le1.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_cclk_configure() function...OR these together to form the cclk_config_options argument
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_16_MHZ				(0x00)
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_8_MHZ				(0x01 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_4_MHZ				(0x02 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_2_MHZ				(0x03 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_1_MHZ				(0x04 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_500_KHZ				(0x05 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_250_KHZ				(0x06 )
#define PWR_CLK_MGMT_CCLK_CLK_FREQ_125_KHZ				(0x07 )

#define PWR_CLK_MGMT_CCLK_WKUP_INT_ON_XOSC16M_DISABLE		(0)
#define PWR_CLK_MGMT_CCLK_WKUP_INT_ON_XOSC16M_ENABLE		(CLKCTRL_XOSC16M_ACTIVE_WKUP_INT_ENABLE)

#define PWR_CLK_MGMT_CCLK_START_XOSC16M_AND_RCOSC16M		(0x00)
#define PWR_CLK_MGMT_CCLK_START_ONLY_RCOSC16M				(0x01 << CLKCTRL_XOSC16M_RCOSC16M_START_SHIFT)
#define PWR_CLK_MGMT_CCLK_START_ONLY_XOSC16M				(0x02 << CLKCTRL_XOSC16M_RCOSC16M_START_SHIFT)

#define PWR_CLK_MGMT_CCLK_CLK_SRC_XOSC16M_OR_RCOSC16M		(0)
#define PWR_CLK_MGMT_CCLK_CLK_SRC_PIN_XC1					(CLKCTRL_CLK_SRC_PIN_XC1_OR_OSC)

#define PWR_CLK_MGMT_CCLK_XOSC16M_IN_REGISTER_RET_OFF		(0)
#define PWR_CLK_MGMT_CCLK_XOSC16M_IN_REGISTER_RET_ON		(CLKCTRL_XOSC16M_IN_REG_RET_MODE_ENABLE)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_clklf_configure() function...use only one to form the clklf_config_options argument
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_XOSC32K				(0x00)
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_RCOSC32K				(0x01 )
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_XOSC16M				(0x02 )
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_PIN_XC1_TO_XOSC32K	(0x03 )
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_PIN_DIG_RAIL_TO_RAIL	(0x04 )
#define PWR_CLK_MGMT_CLKLF_CLK_SRC_NONE					(0x07 )


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_wakeup_sources_configure() function...OR these together to form the wakeup_sources_config_options argument
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_MISCIRQ_IF_INT_ENABLED	(0x00)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_MISCIRQ_ALWAYS			(0x02 )
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_MISCIRQ_NEVER				(0x03 )

#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_WUOPIRQ_IF_INT_ENABLED	(0x00)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_WUOPIRQ_ALWAYS			(0x02 << WUCON_WAKE_ON_WUOPIRQ_SHIFT)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_WUOPIRQ_NEVER				(0x03 << WUCON_WAKE_ON_WUOPIRQ_SHIFT)

#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RTC2_TICK_IF_INT_ENABLED	(0x00)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RTC2_TICK_ALWAYS			(0x02 << WUCON_WAKE_ON_RTC2_TICK_SHIFT)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RTC2_TICK_NEVER			(0x03 << WUCON_WAKE_ON_RTC2_TICK_SHIFT)

#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RFIRQ_IF_INT_ENABLED		(0x00)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RFIRQ_ALWAYS				(0x02 << WUCON_WAKE_ON_RFIRQ_SHIFT)
#define PWR_CLK_MGMT_WAKEUP_WAKEUP_ON_RFIRQ_NEVER				(0x03 << WUCON_WAKE_ON_RFIRQ_SHIFT)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_wakeup_pins_configure() function...OR these together to form the wakeup_on_pin_config_options argument
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_0_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_0_ENABLE	WUOPCX_PIN0

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_1_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_1_ENABLE	WUOPCX_PIN1

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_2_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_2_ENABLE	WUOPCX_PIN2

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_3_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_3_ENABLE	WUOPCX_PIN3

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_4_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_4_ENABLE	WUOPCX_PIN4

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_5_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_5_ENABLE	WUOPCX_PIN5

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_6_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_6_ENABLE	WUOPCX_PIN6

#define PWR_CLK_MGMT_WAKEUP_INPUT_0_7_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_0_7_ENABLE	WUOPCX_PIN7

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_0_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_0_ENABLE	(WUOPCX_PIN0 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_1_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_1_ENABLE	(WUOPCX_PIN1 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_2_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_2_ENABLE	(WUOPCX_PIN2 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_3_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_3_ENABLE	(WUOPCX_PIN3 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_4_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_4_ENABLE	(WUOPCX_PIN4 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_5_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_5_ENABLE	(WUOPCX_PIN5 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_6_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_6_ENABLE	(WUOPCX_PIN6 << 8)

#define PWR_CLK_MGMT_WAKEUP_INPUT_1_7_DISABLE	(0)
#define PWR_CLK_MGMT_WAKEUP_INPUT_1_7_ENABLE	(WUOPCX_PIN7 << 8)

//////////////////////////////////////////////////////////////////////
// Wakeup config options based on GPIO pin defines for 24-pin variety
//////////////////////////////////////////////////////////////////////
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_24_PIN)
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_0_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_0_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_0_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_0_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_1_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_1_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_1_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_1_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_2_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_2_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_2_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_2_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_3_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_3_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_3_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_3_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_4_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_4_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_4_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_4_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_5_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_5_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_5_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_5_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_6_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_6_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_6_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_6_ENABLE
#endif

//////////////////////////////////////////////////////////////////////
// Wakeup config options based on GPIO pin defines for 32-pin variety
//////////////////////////////////////////////////////////////////////
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_32_PIN)
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_0_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_0_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_0_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_0_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_1_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_1_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_1_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_1_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_2_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_2_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_2_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_2_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_3_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_3_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_3_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_3_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_4_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_4_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_4_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_4_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_5_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_5_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_5_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_5_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_6_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_6_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_6_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_6_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_7_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_7_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P0_7_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_7_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_0_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_0_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_0_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_0_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_1_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_1_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_1_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_1_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_2_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_2_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_2_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_2_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_3_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_3_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_3_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_3_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_4_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_4_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_4_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_4_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_5_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_5_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_5_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_5_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_6_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_6_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_6_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_6_ENABLE
#endif

//////////////////////////////////////////////////////////////////////
// Wakeup config options based on GPIO pin defines for 48-pin variety
//////////////////////////////////////////////////////////////////////
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_0_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_0_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_0_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_0_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_1_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_1_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_1_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_1_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_2_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_2_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_2_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_2_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_3_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_3_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_3_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_3_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_4_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_4_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_4_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_4_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_5_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_5_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_5_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_5_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_6_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_6_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_6_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_6_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_7_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_0_7_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P2_7_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_0_7_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_0_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_0_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_0_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_0_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_1_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_1_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_1_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_1_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_2_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_2_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_2_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_2_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_3_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_3_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_3_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_3_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_4_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_4_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_4_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_4_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_5_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_5_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_5_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_5_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_6_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_6_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P3_6_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_6_ENABLE

#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_7_DISABLE	PWR_CLK_MGMT_WAKEUP_INPUT_1_7_DISABLE
#define PWR_CLK_MGMT_WAKEUP_INPUT_P1_7_ENABLE		PWR_CLK_MGMT_WAKEUP_INPUT_1_7_ENABLE
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_op_mode_configure() function...OR these together to form the op_mode_config_options argument
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_OP_MODE_RUN_WD_NORMALLY					(0)
#define PWR_CLK_MGMT_OP_MODE_RESET_WD_ON_MEM_REG_RET			OPMCON_WATCHDOG_RESET_ENABLE

#define PWR_CLK_MGMT_OP_MODE_RETENTION_LATCH_OPEN				(0)
#define PWR_CLK_MGMT_OP_MODE_RETENTION_LATCH_LOCKED				OPMCON_RETENTION_LATCH_CONTROL

#define PWR_CLK_MGMT_OP_MODE_ALL_WAKEUP_PINS_ACTIVE_HIGH		(0)
#define PWR_CLK_MGMT_OP_MODE_SOME_WAKEUP_PINS_ACTIVE_HIGH		OPMCON_WAKEUP_PINS_POLARITY


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in pwr_clk_mgmt_pwr_failure_configure() function...OR these together to form the pwr_failure_config_options argument
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_PWR_FAILURE_POF_THRESHOLD_2_1V	(0x00)
#define PWR_CLK_MGMT_PWR_FAILURE_POF_THRESHOLD_2_3V	(0x01 << POFCON_POF_THRESHOLD_SHIFT)
#define PWR_CLK_MGMT_PWR_FAILURE_POF_THRESHOLD_2_5V	(0x02 << POFCON_POF_THRESHOLD_SHIFT)
#define PWR_CLK_MGMT_PWR_FAILURE_POF_THRESHOLD_2_7V	(0x03 << POFCON_POF_THRESHOLD_SHIFT)

#define PWR_CLK_MGMT_PWR_FAILURE_POF_DISABLE		(0)
#define PWR_CLK_MGMT_PWR_FAILURE_POF_ENABLE			POFCON_POF_ENABLE

///////////////////////////////////////////////////////////////////////////////
// Defines for function macros below (do not use these in configure functions)
///////////////////////////////////////////////////////////////////////////////
#define PWR_CLK_MGMT_PWRDWN_MODE_ACTIVE					(0x00)
#define PWR_CLK_MGMT_PWRDWN_MODE_DEEP_SLEEP				(0x01 )
#define PWR_CLK_MGMT_PWRDWN_MODE_MEMORY_RET_TMR_OFF		(0x02 )
#define PWR_CLK_MGMT_PWRDWN_MODE_MEMORY_RET_TMR_ON		(0x03 )
#define PWR_CLK_MGMT_PWRDWN_MODE_REGISTER_RET			(0x04 )
#define PWR_CLK_MGMT_PWRDWN_MODE_STANDBY				(0x07 )

#define PWR_CLK_MGMT_RESET_REASON_PWR_UP				(0x00)
#define PWR_CLK_MGMT_RESET_REASON_RST_PIN				(0x01 )
#define PWR_CLK_MGMT_RESET_REASON_WATCHDOG				(0x02 )
#define PWR_CLK_MGMT_RESET_REASON_DEBUGGER				(0x04 )


////////////////////////////
// Function macros
////////////////////////////
// Entering sleep modes clears the status bits that sits on the same PWRDWN register, no need to try to preserve them
#define pwr_clk_mgmt_enter_pwr_mode_active()					PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_ACTIVE				//Enter active mode
#define pwr_clk_mgmt_enter_pwr_mode_deep_sleep()				PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_DEEP_SLEEP			//Enter deep sleep mode
#define pwr_clk_mgmt_enter_pwr_mode_memory_ret_tmr_off()		PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_MEMORY_RET_TMR_OFF	//Enter memory retention timers off mode
#define pwr_clk_mgmt_enter_pwr_mode_memory_ret_tmr_on()			PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_MEMORY_RET_TMR_ON	//Enter memory retention timers on mode
#define pwr_clk_mgmt_enter_pwr_mode_register_ret()				PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_REGISTER_RET		//Enter register retention mode
#define pwr_clk_mgmt_enter_pwr_mode_standby()					PWRDWN = PWR_CLK_MGMT_PWRDWN_MODE_STANDBY			//Enter standby mode

#define pwr_clk_mgmt_is_cclk_src_xosc16m()						(CLKLFCTRL & CLKLFCTRL_IS_CLKLF_SRC_XOSC16M)	//True if CCLK source is XOSC16M, false otherwise
#define pwr_clk_mgmt_is_cclk_src_rcosc16m()						(!pwr_clk_mgmt_is_clklf_src_xosc16m())							//True if CCLK source is RCOSC16M, false otherwise
#define pwr_clk_mgmt_wait_until_cclk_src_is_xosc16m()			while(pwr_clk_mgmt_is_cclk_src_rcosc16m())						//Halts processor until XOSC16M is running CCLK

#define pwr_clk_mgmt_is_clklf_ready()							(CLKLFCTRL & CLKLFCTRL_IS_CLKLF_READY)				//True if CLKLF is ready, false otherwise
#define pwr_clk_mgmt_get_clklf_value()							(CLKLFCTRL & CLKLFCTRL_CLKLF_VAL_READ)			//Returns current value (0 or 1) of CLKLF
#define pwr_clk_mgmt_is_clklf_enabled()							((CLKLFCTRL & CLKLFCTRL_CLKLF_SRC_MASK) != PWR_CLK_MGMT_CLKLF_CLK_SRC_NONE) //True if CLKLF is enabled, false otherwise
#define pwr_clk_mgmt_wait_until_clklf_is_ready()				while(!pwr_clk_mgmt_is_clklf_ready())							//Halts processor until CLKLF is ready

#define pwr_clk_mgmt_get_reset_reason()							(RSTREAS & RSTREAS_RESET_REASON_MASK)									//Gets the reason(s) for previous reset(s)
#define pwr_clk_mgmt_was_prev_reset_pwr_up(rstreas_reg_val)		(rstreas_reg_val & PWR_CLK_MGMT_RESET_REASON_PWR_UP)		//True if a prior reset was due to power up, false otherwise
#define pwr_clk_mgmt_was_prev_reset_rst_pin(rstreas_reg_val)	(rstreas_reg_val & PWR_CLK_MGMT_RESET_REASON_RST_PIN)	//True if a prior reset was due to the reset pin, false otherwise
#define pwr_clk_mgmt_was_prev_reset_watchdog(rstreas_reg_val)	(rstreas_reg_val & PWR_CLK_MGMT_RESET_REASON_WATCHDOG)	//True if a prior reset was due to watchdog timeout, false otherwise
#define pwr_clk_mgmt_was_prev_reset_debugger(rstreas_reg_val)	(rstreas_reg_val & PWR_CLK_MGMT_RESET_REASON_DEBUGGER)	//True if a prior reset was due to the debugger, false otherwise
#define pwr_clk_mgmt_clear_reset_reasons()						(void)RSTREAS

#define pwr_clk_mgmt_wake_on_pin_reg_0_pins_disable(pin_mask)	(WUOPC0 &= ~(pin_mask))											//Disables pins on WUOPC0 from waking the processor from sleep
#define pwr_clk_mgmt_wake_on_pin_reg_0_pins_enable(pin_mask)	(WUOPC0 |= (pin_mask))											//Enables pins on WUOPC0 to wake the processor from sleep
#define pwr_clk_mgmt_wake_on_pin_reg_1_pins_disable(pin_mask)	(WUOPC1 &= ~(pin_mask))											//Disables pins on WUOPC1 from waking the processor from sleep
#define pwr_clk_mgmt_wake_on_pin_reg_1_pins_enable(pin_mask)	(WUOPC1 |= (pin_mask))											//Enables pins on WUOPC1 to wake the processor from sleep

#define pwr_clk_mgmt_wakeup_pins_all_high_polarity()			(OPMCON &= ~(OPMCON_WAKEUP_PINS_POLARITY))						//Sets all wakeup pins to wake the device when they go to a high level
#define pwr_clk_mgmt_wakeup_pins_some_low_polarity()			(OPMCON |= ~(OPMCON_WAKEUP_PINS_POLARITY))						//Allows a subset of pins to wake the device on a low level, with the rest of the wakeup pins waking the device when they go to high

#define pwr_clk_mgmt_open_retention_latches()					(OPMCON &= ~(OPMCON_RETENTION_LATCH_CONTROL))					//Opens the I/O latches to allow the user to program their values (normal behavior)
#define pwr_clk_mgmt_close_retention_latches()					(OPMCON |= (OPMCON_RETENTION_LATCH_CONTROL))					//Locks the I/O latches to allow pins to keep their configurations in sleep modes

#define pwr_clk_mgmt_is_vdd_below_bor_threshold()				(POFCON & POFCON_POF_WARNING)					//True if VDD is below brown-out reset threshold, false otherwise


#define PWR_CLK_MGMT_CLKLF_CLKLFCTRL_MASK		(0x07)	//Mask for the options used for CLKLFCTRL
#define PWR_CLK_MGMT_OP_MODE_OPMCON_MASK		(0x07)	//Mask for the options used for OPMCON
#define PWR_CLK_MGMT_CLKLF_POFCON_MASK			(0xF0)	//Mask for the options used for POFCON


////////////////////////////
// Function prototypes
////////////////////////////
uint32_t get_cclk_freq_in_hz();
uint16_t get_cclk_freq_in_khz();
uint8_t get_cclk_freq_in_mhz(); //clocks per us

static inline void pwr_clk_mgmt_cclk(uint8_t cclk_config)
{
	//Set up CLKCTRL register from cclk_config_options
	CLKCTRL = cclk_config;
}

static inline void pwr_clk_mgmt_clklf(uint8_t clklf_config)
{
	//Set up CLKLFCTRL register from clklf_config_options
	CLKLFCTRL = (CLKLFCTRL & ~PWR_CLK_MGMT_CLKLF_CLKLFCTRL_MASK) | (clklf_config & PWR_CLK_MGMT_CLKLF_CLKLFCTRL_MASK);
}

static inline void pwr_clk_mgmt_op_mode(uint8_t op_mode_config)
{
	//Set up OPMCON register from op_mode_config_options
	OPMCON = (OPMCON & ~PWR_CLK_MGMT_OP_MODE_OPMCON_MASK) | (op_mode_config & PWR_CLK_MGMT_OP_MODE_OPMCON_MASK);
}

static inline void pwr_clk_mgmt_pwr_failure(uint8_t pwr_failure_config)
{
	//Set up POFCON register from pwr_failure_config_options
	POFCON = (POFCON & ~PWR_CLK_MGMT_CLKLF_POFCON_MASK) | (pwr_failure_config & PWR_CLK_MGMT_CLKLF_POFCON_MASK);
}

static inline void pwr_clk_mgmt_wakeup_pins(uint16_t wakeup_on_pin_config)
{
	//Set up WUOPCx registers from wakeup_on_pin_config_options
	WUOPC0 = wakeup_on_pin_config & 0xFF;
	WUOPC1 = (wakeup_on_pin_config >> 8) & 0xFF;
}

static inline void pwr_clk_mgmt_wakeup_sources(uint8_t wakeup_sources_config)
{
	//Set up WUCON register from wakeup_sources_config_options
	WUCON = wakeup_sources_config;
}


#endif /* PWR_CLK_MGMT_H_ */
