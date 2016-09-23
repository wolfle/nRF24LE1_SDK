#include "uart.h"
#include "pwr_clk_mgmt.h"
#include "timer.h"
#include "interrupt.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uint32_t uart_calc_actual_baud_rate_from_s0rel(uint16_t s0rel_reg_value, bool smod_bit_value)
//
// Description:
//  Calculates a baud rate from the passed in values for S0REL and SMOD
//
// Parameters:
//  uint16_t s0rel_reg_value - value of S0REL to use in the calculation
//  bool smod_bit_value - value of SMOD to use in the calculation
//
// Return value:
//  Actual baud rate (subject to rounding errors) calculated given S0REL and SMOD values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t uart_calc_actual_baud_rate_from_s0rel(uint16_t s0rel_reg_value, bool smod_bit_value)
{
	//Calculates the actual baud rate from the formula
	//
	//     (2 ^ SMOD) * CCLK_FREQ
	//BR = ----------------------
	//       64 * (1024 - S0REL)
	//
	//The formula below also rounds to the nearest whole number
	return (uint32_t)(((((uint32_t)(2 * ((smod_bit_value != 0) ? 2 : 1) * get_cclk_freq_in_hz())) / ((uint32_t)(64 * (1024 - s0rel_reg_value)))) + 1) / 2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uint32_t uart_calc_actual_baud_rate_from_th1(uint8_t th1_reg_value, bool smod_bit_value)
//
// Description:
//  Calculates a baud rate from the passed in values for TH1 and SMOD
//
// Parameters:
//  uint16_t th1_reg_value - value of TH1 to use in the calculation
//  bool smod_bit_value - value of SMOD to use in the calculation
//
// Return value:
//  Actual baud rate (subject to rounding errors) calculated given TH1 and SMOD values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t uart_calc_actual_baud_rate_from_th1(uint8_t th1_reg_value, bool smod_bit_value)
{
	//Calculates the actual baud rate from the formula
	//
	//     (2 ^ SMOD) * CCLK_FREQ
	//BR = ----------------------
	//       384 * (256 - TH1)
	//
	//The formula below also rounds to the nearest whole number
	return (uint32_t)(((((uint32_t)(2 * ((smod_bit_value != 0) ? 2 : 1) * get_cclk_freq_in_hz())) / ((uint32_t)(384 * (256 - th1_reg_value)))) + 1) / 2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uint16_t uart_calc_s0rel_value(uint32_t desired_baud_rate, bool smod_bit_value)
//
// Description:
//  Calculates a value for S0REL from the passed in values for baud rate and SMOD
//
// Parameters:
//  uint32_t desired_baud_rate - baud rate use in the calculation
//  bool smod_bit_value - value of SMOD to use in the calculation
//
// Return value:
//  Value to use for S0REL based on the desired baud rate and SMOD value
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t uart_calc_s0rel_value(uint32_t desired_baud_rate, bool smod_bit_value)
{
	//Calculates the S0REL value from the desired baud rate from the formula
	//
	//               (2 ^ SMOD) * CCLK_FREQ
	//S0REL = 1024 - ----------------------
	//               64 * desired_baud_rate
	//
	//The formula below also rounds to the nearest whole number
	return (uint16_t)(1024 - (((((uint32_t)(2 * ((smod_bit_value != 0) ? 2 : 1) * get_cclk_freq_in_hz())) / ((uint32_t)(64 * desired_baud_rate))) + 1) / 2));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uint16_t uart_calc_th1_value(uint32_t desired_baud_rate, bool smod_bit_value)
//
// Description:
//  Calculates a value for TH1 from the passed in values for baud rate and SMOD
//
// Parameters:
//  uint32_t desired_baud_rate - baud rate use in the calculation
//  bool smod_bit_value - value of SMOD to use in the calculation
//
// Return value:
//  Value to use for TH1 based on the desired baud rate and SMOD value
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t uart_calc_th1_value(uint32_t desired_baud_rate, bool smod_bit_value)
{
	//Calculates the TH1 value from the desired baud rate from the formula
	//
	//            (2 ^ SMOD) * CCLK_FREQ
	//TH1 = 256 - -----------------------
	//            384 * desired_baud_rate
	//
	//The formula below also rounds to the nearest whole number
	return (uint8_t)(256 - (((((uint32_t)(2 * ((smod_bit_value != 0) ? 2 : 1) * get_cclk_freq_in_hz())) / ((uint32_t)(384 * desired_baud_rate))) + 1) / 2));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void uart_configure_auto_baud_calc(uint8_t uart_config_options, uint32_t baud_rate)
//
// Description:
//  Configures the UART and calculates all values based on desired baud rate
//
// Parameters:
//  uint8_t uart_config_options - UART configuration options
//  uint32_t desired_baud_rate - desired baud rate
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart_configure_auto_baud_calc(uint8_t uart_config_options, uint32_t baud_rate)
{
	//We only need to configure the baud rate for modes 1 and 3
	if(((uart_config_options & UART_MODE_MASK) == UART_MODE_1_UART_8_BIT) ||
	   ((uart_config_options & UART_MODE_MASK) == UART_MODE_3_UART_9_BIT))
	{
		uint32_t calc_br_with_smod_cleared, calc_br_with_smod_set;
		uint32_t error_calc_br_with_smod_cleared, error_calc_br_with_smod_set;

		//Calculate values based on whether the request is for the BR generator or timer 1
		if((uart_config_options & UART_CLOCK_MASK) == UART_CLOCK_FOR_MODES_1_3_USE_BR_GEN)
		{
			uint16_t s0rel_with_smod_cleared, s0rel_with_smod_set;

			//Calculate the values of S0REL with SMOD cleared and set
			s0rel_with_smod_cleared = uart_calc_s0rel_value(baud_rate, 0);
			s0rel_with_smod_set = uart_calc_s0rel_value(baud_rate, 1);

			//Plug the above values back into the baud rate calculation to see what the actual BR is with each
			calc_br_with_smod_cleared = uart_calc_actual_baud_rate_from_s0rel(s0rel_with_smod_cleared, false);
			calc_br_with_smod_set = uart_calc_actual_baud_rate_from_s0rel(s0rel_with_smod_set, true);

			//Calculate the magnitude of the error of the baud rate with each of the calculated baud rates versus the requested one
			error_calc_br_with_smod_cleared = (calc_br_with_smod_cleared > baud_rate) ? (calc_br_with_smod_cleared - baud_rate) : (baud_rate - calc_br_with_smod_cleared);
			error_calc_br_with_smod_set = (calc_br_with_smod_set > baud_rate) ? (calc_br_with_smod_set - baud_rate) : (baud_rate - calc_br_with_smod_set);

			//Use the values that had the least error (tie goes to SMOD set)
			if(error_calc_br_with_smod_cleared < error_calc_br_with_smod_set)
			{
				uart_configure_manual_baud_calc(((uart_config_options & ~UART_BIT_SMOD_MASK) | UART_BIT_SMOD_CLEAR),
												s0rel_with_smod_cleared);
			}
			else
			{
				uart_configure_manual_baud_calc(((uart_config_options & ~UART_BIT_SMOD_MASK) | UART_BIT_SMOD_SET),
												s0rel_with_smod_set);
			}
		}
		else
		{
			uint8_t th1_with_smod_cleared, th1_with_smod_set;

			//Calculate the values of TH1 with SMOD cleared and set
			th1_with_smod_cleared = uart_calc_th1_value(baud_rate, 0);
			th1_with_smod_set = uart_calc_th1_value(baud_rate, 1);

			//Plug the above values back into the baud rate calculation to see what the actual BR is with each
			calc_br_with_smod_cleared = uart_calc_actual_baud_rate_from_th1(th1_with_smod_cleared, false);
			calc_br_with_smod_set = uart_calc_actual_baud_rate_from_th1(th1_with_smod_set, true);

			//Calculate the magnitude of the error of the baud rate with each of the calculated baud rates versus the requested one
			error_calc_br_with_smod_cleared = (calc_br_with_smod_cleared > baud_rate) ? (calc_br_with_smod_cleared - baud_rate) : (baud_rate - calc_br_with_smod_cleared);
			error_calc_br_with_smod_set = (calc_br_with_smod_set > baud_rate) ? (calc_br_with_smod_set - baud_rate) : (baud_rate - calc_br_with_smod_set);

			//Use the values that had the least error (tie goes to SMOD set)
			if(error_calc_br_with_smod_cleared < error_calc_br_with_smod_set)
			{
				uart_configure_manual_baud_calc(((uart_config_options & ~UART_BIT_SMOD_MASK) | UART_BIT_SMOD_CLEAR),
												(uint16_t)th1_with_smod_cleared);
			}
			else
			{
				uart_configure_manual_baud_calc(((uart_config_options & ~UART_BIT_SMOD_MASK) | UART_BIT_SMOD_SET),
												(uint16_t)th1_with_smod_set);
			}
		}
	}
}

///////////////////////////////////////////
// Internal defines
///////////////////////////////////////////
#define UART_S0CON_MASK	0xF0	//Mask for the options used for S0CON


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void uart_configure_manual_baud_calc(uint8_t uart_config_options, uint16_t s0rel_val_if_br_gen_or_th1_val_if_tmr1)
//
// Description:
//  Configures the UART
//
// Parameters:
//  uint8_t uart_config_options - UART configuration options
//  uint16_t s0rel_val_if_br_gen_or_th1_val_if_tmr1 - value of S0REL if using the hardware baud rate generator or the value of
//   TH1 if using timer 1
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart_configure_manual_baud_calc(uint8_t uart_config_options, uint16_t s0rel_val_if_br_gen_or_th1_val_if_tmr1)
{
	//Set up SMOD as requested
	if(uart_config_options & UART_BIT_SMOD_SET)
	{
		reg_bits_set(PCON, PCON_SMOD);
	}
	else
	{
		reg_bits_clear(PCON, PCON_SMOD);
	}

	//Set up the UART if one of the two modes that use baud rate generation is requested
	if(((uart_config_options & UART_MODE_MASK) == UART_MODE_1_UART_8_BIT) ||
	   ((uart_config_options & UART_MODE_MASK) == UART_MODE_3_UART_9_BIT))
	{
		//Set up based on whether the baud rate generator or timer 1 is being used to control timing
		if((uart_config_options & UART_CLOCK_MASK) == UART_CLOCK_FOR_MODES_1_3_USE_BR_GEN)
		{
			S0REL = s0rel_val_if_br_gen_or_th1_val_if_tmr1; //set S0REL to the value requested
			sbit_set(ADCON_SB_BD); //enable BR generator
		}
		else
		{
			sbit_clear(ADCON_SB_BD); //disable BR generator and use TMR1 overflow

			//Configure timer 1 for use as the baud rate generator
			timer1_init(TIMER1_MODE_2_8_BIT_AUTO_RLD_TMR |
							 TIMER1_FUNCTION_TIMER |
							 TIMER1_GATE_ALWAYS_RUN_TIMER,
							 (uint8_t)s0rel_val_if_br_gen_or_th1_val_if_tmr1,0);

//			timer1_run(); //enable the timer
		}
	}

	//Set up S0CON register from uart_config_options
	S0CON = uart_config_options & UART_S0CON_MASK;

	// Prepare interrupt flags
	irq_flag_set(irq_flag_uart_tx);  // Pretend last character has been sent.
	irq_flag_clear(irq_flag_uart_rx);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void uart_send_wait_for_complete(uint8_t tx_data)
//
// Description:
//  Sends a byte over the UART and waits for it to complete
//
// Parameters:
//  uint8_t tx_data - byte to send over the UART
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart_send_wait_for_complete(uint8_t tx_data)
{
//	irq_clear_flag(irq_flag_uart_tx);

        uart_send(tx_data);

	irq_wait_flag(irq_flag_uart_tx);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uint8_t uart_wait_for_rx_and_get()
//
// Description:
//  Waits until a byte is received over the UART and returns the value
//
// Parameters:
//  None
//
// Return value:
//  Value received over UART
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t uart_wait_for_rx_and_get()
{
	irq_wait_flag(irq_flag_uart_rx);

	return uart_get();
}
