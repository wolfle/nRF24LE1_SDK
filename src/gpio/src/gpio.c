#include "gpio.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_configure(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Configures a GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be configured
//  uint8_t gpio_pin_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_configure(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		//The pin is in P0, so now check if the request is for input or output direction
		if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpio_pins_val_set(P0, (1 << (gpio_pin_id % 8)));
			}
			else
			{
				gpio_pins_val_clear(P0, (1 << (gpio_pin_id % 8)));
			}

			//Set up the options (other than the actual direction) for the pin here
			P0CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an output
			gpio_pins_dir_output(P0DIR, (1 << (gpio_pin_id % 8)));
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P0CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_input(P0DIR, (1 << (gpio_pin_id % 8)));
		}
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		//The pin is in P1, so now check if the request is for input or output direction
		if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpio_pins_val_set(P1, (1 << (gpio_pin_id % 8)));
			}
			else
			{
				gpio_pins_val_clear(P1, (1 << (gpio_pin_id % 8)));
			}

			//Set up the options (other than the actual direction) for the pin here
			P1CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_output(P1DIR, (1 << (gpio_pin_id % 8)));
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P1CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_input(P1DIR, (1 << (gpio_pin_id % 8)));
		}
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		//The pin is in P2, so now check if the request is for input or output direction
		if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpio_pins_val_set(P2, (1 << (gpio_pin_id % 8)));
			}
			else
			{
				gpio_pins_val_clear(P2, (1 << (gpio_pin_id % 8)));
			}

			//Set up the options (other than the actual direction) for the pin here
			P2CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_output(P2DIR, (1 << (gpio_pin_id % 8)));
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P2CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_input(P2DIR, (1 << (gpio_pin_id % 8)));
		}
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		//The pin is in P3 (keep in mind there is no P3.7 on any part), so now check if the request is for input or output direction
		if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_DIR_OUTPUT)
		{
			//The request was output, so now check if the pin value should be cleared or set
			if(gpio_pin_config_options & GPIO_PIN_CONFIG_OPTION_OUTPUT_VAL_SET)
			{
				gpio_pins_val_set(P3, (1 << (gpio_pin_id % 8)));
			}
			else
			{
				gpio_pins_val_clear(P3, (1 << (gpio_pin_id % 8)));
			}

			//Set up the options (other than the actual direction) for the pin here
			P3CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_output(P3DIR, (1 << (gpio_pin_id % 8)));
		}
		else
		{
			//The request was input, so set up the options (other than the actual direction) for the pin here
			P3CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
					PXCON_IN_OUT |
					(gpio_pin_config_options & PXCON_PINMODE_MASK);

			//Finally, set up the GPIO pin as an input
			gpio_pins_dir_input(P3DIR, (1 << (gpio_pin_id % 8)));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_dir_input(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Configures a GPIO pin as an input
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be configured as an input
//  uint8_t gpio_pin_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_dir_input(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		gpio_pins_dir_input(P0DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P0CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		gpio_pins_dir_input(P1DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P1CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		gpio_pins_dir_input(P2DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P2CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		gpio_pins_dir_input(P2DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P3CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_dir_output(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Configures a GPIO pin as an output
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be configured as an output
//  uint8_t gpio_pin_config_options - GPIO pin configuration options
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_dir_output(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		gpio_pins_dir_output(P0DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P0CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		gpio_pins_dir_output(P1DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P1CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		gpio_pins_dir_output(P2DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P2CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		gpio_pins_dir_output(P3DIR, (1 << (gpio_pin_id % 8)));

		//Set up the options (other than the actual direction) for the pin here
		P3CON = ((gpio_pin_id % 8) & PXCON_BIT_ADDR_MASK) |
				(gpio_pin_config_options & PXCON_PINMODE_MASK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_val_clear(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Clears a GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be cleared
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_val_clear(gpio_pin_id_t gpio_pin_id)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		gpio_pins_val_clear(P0, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		gpio_pins_val_clear(P1, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		gpio_pins_val_clear(P2, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		gpio_pins_val_clear(P3, (1 << (gpio_pin_id % 8)));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_val_complement(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Complements a GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be complemented
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_val_complement(gpio_pin_id_t gpio_pin_id)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		gpio_pins_val_complement(P0, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		gpio_pins_val_complement(P1, (1 << (gpio_pin_id % 8)));

	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		gpio_pins_val_complement(P2, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		gpio_pins_val_complement(P3, (1 << (gpio_pin_id % 8)));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// bool gpio_pin_val_read(gpio_pin_id_t gpio_pin_id)
//
// Description:
//  Returns the value of the requested GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be read
//
// Return value:
//  The value of the requested GPIO pin
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool gpio_pin_val_read(gpio_pin_id_t gpio_pin_id)
{
	bool value = false;

	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		value = P0 & (1 << (gpio_pin_id % 8));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		value = P1 & (1 << (gpio_pin_id % 8));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		value = P2 & (1 << (gpio_pin_id % 8));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		value = P3 & (1 << (gpio_pin_id % 8));
	}

	if(value)
	{
		return true;
	}
	else
	{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_val_set(gpio_pin_id_t gpio_pin_id, uint8_t gpio_pin_config_options)
//
// Description:
//  Sets a GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be set
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_val_set(gpio_pin_id_t gpio_pin_id)
{
	//Process the request based on the pin block
	if(gpio_pin_id <= GPIO_PIN_ID_P0_7)
	{
		gpio_pins_val_set(P0, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P1_7)
	{
		gpio_pins_val_set(P1, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P2_7)
	{
		gpio_pins_val_set(P2, (1 << (gpio_pin_id % 8)));
	}
	else if(gpio_pin_id <= GPIO_PIN_ID_P3_6)
	{
		gpio_pins_val_set(P3, (1 << (gpio_pin_id % 8)));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// void gpio_pin_val_write(gpio_pin_id_t gpio_pin_id, bool value)
//
// Description:
//  Writes the specified value to a GPIO pin
//
// Parameters:
//  gpio_pin_id_t gpio_pin_id - ID of the GPIO pin to be cleared
//  bool value - value to be written to the GPIO pin
//
// Return value:
//  None
//
///////////////////////////////////////////////////////////////////////////////////////////////
void gpio_pin_val_write(gpio_pin_id_t gpio_pin_id, bool value)
{
	//Process the request based on the specified value
	if(value)
	{
		gpio_pin_val_set(gpio_pin_id);
	}
	else
	{
		gpio_pin_val_clear(gpio_pin_id);
	}
}
