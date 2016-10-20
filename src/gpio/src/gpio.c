#include "gpio.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_configure(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Configures a GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be configured
//  uint8_t gpio_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_configure(gpio_id_t gpio_id, uint8_t gpio_config_options)
{
	uint8_t g=gpio_id & 0x7, m=(1<<g);
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		//The pin is in P0, so now check if the request is for input or output direction
		if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpios_set(P0, m);
			}
			else
			{
				gpios_clear(P0, m);
			}

			//Set up the options (other than the actual direction) for the pin here
			P0CON = (g & PXCON_BIT_ADDR_MASK) |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an output
			gpios_dir_output(P0DIR, m);
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P0CON = (g & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_input(P0DIR, m);
		}
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		//The pin is in P1, so now check if the request is for input or output direction
		if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpios_set(P1, m);
			}
			else
			{
				gpios_clear(P1, m);
			}

			//Set up the options (other than the actual direction) for the pin here
			P1CON = (g & PXCON_BIT_ADDR_MASK) |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_output(P1DIR, m);
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P1CON = (g & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_input(P1DIR, m);
		}
	}
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		//The pin is in P2, so now check if the request is for input or output direction
		if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpios_set(P2, m);
			}
			else
			{
				gpios_clear(P2, m);
			}

			//Set up the options (other than the actual direction) for the pin here
			P2CON = (g & PXCON_BIT_ADDR_MASK) |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_output(P2DIR, m);
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P2CON = (g & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_input(P2DIR, m);
		}
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		//The pin is in P3 (keep in mind there is no P3.7 on any part), so now check if the request is for input or output direction
		if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpios_set(P3, m);
			}
			else
			{
				gpios_clear(P3, m);
			}

			//Set up the options (other than the actual direction) for the pin here
			P3CON = (g & PXCON_BIT_ADDR_MASK) |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_output(P3DIR, m);
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P3CON = (g & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpios_dir_input(P3DIR, m);
		}
	}
#endif
}
/*
///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_dir_input(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Configures a GPIO pin as an input
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be configured as an input
//  uint8_t gpio_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_dir_input(gpio_id_t gpio_id, uint8_t gpio_config_options)
{
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		gpios_dir_input(P0DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P0CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		gpios_dir_input(P1DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P1CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		gpios_dir_input(P2DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P2CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		gpios_dir_input(P2DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P3CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_dir_output(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Configures a GPIO pin as an output
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be configured as an output
//  uint8_t gpio_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_dir_output(gpio_id_t gpio_id, uint8_t gpio_config_options)
{
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		gpios_dir_output(P0DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P0CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		gpios_dir_output(P1DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P1CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		gpios_dir_output(P2DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P2CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		gpios_dir_output(P3DIR, (1 << (gpio_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P3CON = ((gpio_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_config_options & PXCON_PINMODE_MASK);
	}
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_clear(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Clears a GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be cleared
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_clear(gpio_id_t gpio_id)
{
	uint8_t m=(1 << (gpio_id & 0x7));
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		gpios_clear(P0, m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		gpios_clear(P1, m);
	}
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		gpios_clear(P2, m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		gpios_clear(P3, m);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_complement(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Complements a GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be complemented
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_complement(gpio_id_t gpio_id)
{
	uint8_t m=(1 << (gpio_id & 0x7));
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		gpios_complement(P0, m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		gpios_complement(P1, m);

	}
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		gpios_complement(P2, m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		gpios_complement(P3, m);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// bool gpio_read(gpio_id_t gpio_id)
//
// Description:
//  Returns the value of the requested GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be read
//
// Return value:
//  The value of the requested GPIO pin
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool gpio_read(gpio_id_t gpio_id)
{
	bool value = false;
	uint8_t m=(1 << (gpio_id & 0x7));
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		value = P0 & m;
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		value = P1 & m;
	}
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		value = P2 & m;
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		value = P3 & m;
	}
#endif
	return (value);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_set(gpio_id_t gpio_id, uint8_t gpio_config_options)
//
// Description:
//  Sets a GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be set
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_set(gpio_id_t gpio_id)
{
	uint8_t m=(1 << (gpio_id & 0x7));
	//Process the request based on the pin block
	if(gpio_id <= GPIO_PIN_ID_P0_7)
	{
		gpios_set(P0,m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P1_7)
	{
		gpios_set(P1,m);
	}
#if (__TARG_PACKAGE_TYPE == NRF24LE1_PACKAGE_48_PIN)
	else if(gpio_id <= GPIO_PIN_ID_P2_7)
	{
		gpios_set(P2,m);
	}
	else if(gpio_id <= GPIO_PIN_ID_P3_6)
	{
		gpios_set(P3, m);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_write(gpio_id_t gpio_id, bool value)
//
// Description:
//  Writes the specified value to a GPIO pin
//
// Parameters:
//  gpio_id_t gpio_id - ID of the GPIO pin to be cleared
//  bool value - value to be written to the GPIO pin
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_write(gpio_id_t gpio_id, bool value)
{
	//Process the request based on the specified value
	if(value)
	{
		gpio_set(gpio_id);
	}
	else
	{
		gpio_clear(gpio_id);
	}
}
