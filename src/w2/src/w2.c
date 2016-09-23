#include "gpio.h"
#include "reg24le1.h"
#include "interrupt.h"
#include "delay.h"
#include "w2.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration defines for use in w2_configure() function...OR these together to form the w2_config_options argument
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define W2_CFG_DISABLE							(0)																					//Disable W2
#define W2_CFG_ENABLE								((W2CON0_ENABLE) << W2_CFG_W2CON0_OFFSET_SHIFT)							//Enable W2

#define W2_CFG_MODE_SLAVE							(0)																					//Run as a slave
#define W2_CFG_MODE_MASTER						((W2CON0_MASTER_SELECT) << W2_CFG_W2CON0_OFFSET_SHIFT)					//Run as a master

#define W2_CFG_FREQ_IDLE					((0x00 << W2CON0_CLOCK_FREQUENCY_SHIFT) << W2_CFG_W2CON0_OFFSET_SHIFT)	//Keep clock idle
#define W2_CFG_FREQ_100_KHZ					((0x01 << W2CON0_CLOCK_FREQUENCY_SHIFT) << W2_CFG_W2CON0_OFFSET_SHIFT)	//Run clock at 100 kHz (requires system clock of >= 4 MHz)
#define W2_CFG_FREQ_400_KHZ					((0x02 << W2CON0_CLOCK_FREQUENCY_SHIFT) << W2_CFG_W2CON0_OFFSET_SHIFT)	//Run clock at 400 kHz (requires system clock of >= 8 MHz)

#define W2_CFG_SLAVE_IRQ_ON_ADDR_MATCH_ENABLE		(0)																					//For slaves, enable interrupt on address match
#define W2_CFG_SLAVE_IRQ_ON_ADDR_MATCH_DISABLE	((W2CON0_SLAVE_DISABLE_IRQ_ON_ADDR_MATCH) << W2_CFG_W2CON0_OFFSET_SHIFT)	//For slaves, disable interrupt on address match

#define W2_CFG_SLAVE_IRQ_ON_STOP_ENABLE			(0)																					//For slaves, enable interrupt on reception of stop condition
#define W2_CFG_SLAVE_IRQ_ON_STOP_DISABLE			((W2CON0_SLAVE_DISABLE_IRQ_ON_STOP) << W2_CFG_W2CON0_OFFSET_SHIFT)		//For slaves, disable interrupt on reception of stop condition

#define W2_CFG_SLAVE_CLOCK_ACTIVE_BETWEEN_XFERS	(0)																					//For slaves, keep clock low between transmitted bytes (clock stretching)
#define W2_CFG_SLAVE_CLOCK_LOW_BETWEEN_XFERS		((W2CON0_SLAVE_CLOCK_STOP) << W2_CFG_W2CON0_OFFSET_SHIFT)				//For slaves, keep clock running between transmitted bytes

#define W2_CFG_SLAVE_BROADCAST_DISABLE			(0)																					//For slaves, do not respond to broadcast address (0x00)
#define W2_CFG_SLAVE_BROADCAST_ENABLE				((W2CON0_SLAVE_BROADCAST_ENABLE) << W2_CFG_W2CON0_OFFSET_SHIFT)			//For slaves, respond to broadcast address (0x00)

#define	W2_CFG_IRQ_ENABLE				(0)																					//Allow interrupts that are enabled in W2CON0
#define	W2_CFG_IRQ_DISABLE				((W2CON1_DISABLE_INTERRUPTS) << W2_CFG_W2CON1_OFFSET_SHIFT)				//Disable all interrupts, whether they are enabled in W2CON0 or not


#define 	W2_ACK_VAL    0
#define 	W2_NACK_VAL  1

#define		W2_MASTER_WRITE  0x00
#define		W2_MASTER_READ   0x01

////////////////////////////
// Function macros
////////////////////////////
#define w2_is_in_master_mode()			((W2CON0 & W2CON0_MASTER_SELECT) != 0 ? true : false)											//Returns true if running in master mode, false if running in slave mode
#define w2_send_start_condition()		W2CON0 |= W2CON0_MASTER_TX_START																//Transmit a start condition after any pending TX data or stop condition
#define w2_send_stop_condition()		W2CON0 |= W2CON0_MASTER_TX_STOP																	//Transmit a stop condition in RX after current byte is received, or in TX after any pending TX data is transmitted
#define w2_send_byte(tx_byte)			W2DAT = (tx_byte)																				//Send a byte over the bus
#define w2_get_rxed_byte()				(W2DAT)																							//Read the last byte received over the bus
#define w2_wait_for_irq()				irq_wait_flag(irq_flag_spi_2wire)																		//Wait for and clear the W2 interrupt
#define w2_master_set_clock_idle()		W2CON0 = ((W2CON0 & (~W2CON0_CLOCK_FREQUENCY_MASK)) | (0x00 << W2CON0_CLOCK_FREQUENCY_SHIFT))	//Set W2 clock to idle
#define w2_master_set_clock_100_khz()	W2CON0 = ((W2CON0 & (~W2CON0_CLOCK_FREQUENCY_MASK)) | (0x01 << W2CON0_CLOCK_FREQUENCY_SHIFT))	//Set W2 clock to 100 kHz
#define w2_master_set_clock_400_khz()	W2CON0 = ((W2CON0 & (~W2CON0_CLOCK_FREQUENCY_MASK)) | (0x02 << W2CON0_CLOCK_FREQUENCY_SHIFT))	//Set W2 clock to 400 kHz


//Since W2CON1[4:0] all get cleared on a read of W2CON1, it is important to read the value of W2CON1 into a variable and use this variable in these macros when looking
// at more than one bit flag.  Alternately, if only one bit flag is ever being polled, W2CON1 can be inserted as the argument of the macro and no buffer is needed.
#define w2_data_txed_or_rxed(w2con1_value)				((w2con1_value & W2CON1_DATA_READY) != 0 ? true : false)						//Returns true if byte has finished transmitting/has been received, false otherwise
#define w2_get_last_rxed_ack(w2con1_value)				((w2con1_value & W2CON1_LAST_ACK) != 0 ? W2_NACK_VAL : W2_ACK_VAL)				//Returns the value of the last received acknowledgement (1 for NACK and 0 for ACK)
#define w2_irq_caused_by_addr_match(w2con1_value)		((w2con1_value & W2CON1_SLAVE_IRQ_DUE_TO_ADDR_MATCH) != true ? 1 : false)		//Returns true if IRQ was caused by an address match, false otherwise
#define w2_irq_caused_by_stop_cond(w2con1_value)		((w2con1_value & W2CON1_SLAVE_IRQ_DUE_TO_STOP) != true ? 1 : false)				//Returns true if IRQ was caused by a stop condition, false otherwise
#define w2_last_rxed_addr_was_broadcast(w2con1_value)	((w2con1_value & W2CON1_SLAVE_LAST_ADDR_WAS_BROADCAST) != true ? 1 : false)		//Returns true if last received address was the broadcast address (0x00), false otherwise

////////////////////////////
#define w2_prepare_scl_sda()	gpio_pin_val_sbit_clear(GPIO_PIN_SBIT_FUNC_W2SCL);\
								gpio_pin_val_sbit_clear(GPIO_PIN_SBIT_FUNC_W2SDA)						//Clear the GPIO values of the pins
#define w2_clear_sda()			gpio_pins_dir_output(GPIO_PIN_REG_FUNC_W2SDA, GPIO_PIN_MASK_FUNC_W2SDA)	//This causes the bus to see a value of 0 (call w2_prepare_scl_sda() first)
#define w2_set_sda()			gpio_pins_dir_input(GPIO_PIN_REG_FUNC_W2SDA, GPIO_PIN_MASK_FUNC_W2SDA)	//This causes the bus to see a value of 1
#define w2_read_sda()			(GPIO_PIN_SBIT_FUNC_W2SDA)												//Returns the current value of W2SDA
#define w2_clear_scl()			gpio_pins_dir_output(GPIO_PIN_REG_FUNC_W2SCL, GPIO_PIN_MASK_FUNC_W2SCL)	//This causes the bus to see a value of 0 (call w2_prepare_scl_sda() first)
#define w2_set_scl()			gpio_pins_dir_input(GPIO_PIN_REG_FUNC_W2SCL, GPIO_PIN_MASK_FUNC_W2SCL)	//This causes the bus to see a value of 1
#define w2_read_scl()			(GPIO_PIN_SBIT_FUNC_W2SCL)												//Returns the current value of W2SCL

#define w2_is_reading(addr) ((addr)&W2_MASTER_READ)

static 	uint8_t addr; //and direction bit
static	volatile uint8_t  * buf;  // if nack received, set buf=0
static 	volatile uint16_t len;

void w2_init(uint8_t opt, uint8_t addr_in_slave_mode){
	w2_enable(); //Datasheet v1.6, p. 159 states that you must enable 2 wire before changing anything else
	W2CON0 = W2CON0_ENABLE|
			((opt&0x03)<< W2CON0_CLOCK_FREQUENCY_SHIFT)|
			 (opt&W2_MASTER?W2CON0_MASTER_SELECT:(
				(opt&W2_SIRQ_ADDR_MATCH_DISABLE?W2CON0_SLAVE_DISABLE_IRQ_ON_ADDR_MATCH:0)|
				(opt&W2_SIRQ_STOP_DISABLE?W2CON0_SLAVE_DISABLE_IRQ_ON_STOP:0)|
				(opt&W2_SCLK_LOW_BETWEEN_XFERS?W2CON0_SLAVE_CLOCK_STOP:0)|
				(opt&W2_SBROADCAST_ENABLE?W2CON0_SLAVE_BROADCAST_ENABLE:0)));
	W2CON1 = 0;  // bit 5 for irq mask. clear it to enable irq

	if(!w2_is_in_master_mode())	W2SADR = addr_in_slave_mode;
	if(!opt&W2_ENABLE)w2_disable();
	
	irq_flag_clear(irq_flag_spi_2wire); // Is this neccesary?
	irq_spi_2wire_enable();
	irq_unmask_2wire();
	addr=0;
	buf=0;
	len=0;
}

void w2_irq_handle(void){
	uint8_t w2con1=W2CON1;

	if(w2_data_txed_or_rxed(w2con1)&&len){

		if(w2_is_reading(addr)){
			*buf++=w2_get_rxed_byte();
			--len;
		}else{ //writing
			if(--len)w2_send_byte(*buf++);
		}
		
		if(len==1)w2_send_stop_condition();
		
		if(w2_get_last_rxed_ack(w2con1)){ //nack received
			if(!w2_is_reading(addr)) w2_send_stop_condition(); //else need to reset w2 after the irq handle return
			buf=0;
		}
		if(!len)buf=0;
	}
}

static void w2_master_io(char isread,uint8_t address, uint8_t * data_buf, uint16_t size){
	addr=((address << W2DAT_ADDRESS_SHIFT) | (isread?1:0));
	buf=data_buf;
	len=size;
	w2_send_start_condition();
	w2_send_byte( addr );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void w2_master_software_reset()
//
// Description:
//  Resets the W2 bus
//  Note: this function is a ported/modified version of hal_w2_soft_reset() from Nordic's nAN-20
//   application note source code
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
static inline void w2_master_software_reset(void){
	uint8_t w2con0_copy;

	//Store the current W2CON0 settings
	w2con0_copy = W2CON0;

	//Clear the actual GPIO values (this only affects the pin value when the pin is configured as an output)
	w2_prepare_scl_sda();

	//Set up for the first part of a stop condition (SCL = 0, SDA = 1)
	//This will take effect after the call to w2_disable() below
	w2_clear_scl();
	w2_set_sda();

	//Reset W2 by bringing the clock to idle and disabling the hardware block
	w2_master_set_clock_idle();
	w2_master_set_clock_100_khz();
	w2_disable();
	delay_us(5);

	//Clear SDA
	w2_clear_sda();
	delay_us(5);

	//Set SCL
	w2_set_scl();
	delay_us(5);

	//Set SDA to complete the stop condition
	w2_set_sda();
	delay_us(5);

	//Return to the W2CON0 settings that were in place before this function was called
	W2CON0 = w2con0_copy;
}

PT(w2_master_read,uint8_t slave_address, uint8_t * data_buf, uint16_t data_len)
{
	PT_B()
		//wait for completion
		PT_WAIT(!buf);
					
		if(data_buf){
			//Send the reading request
			w2_master_io(W2_MASTER_READ,slave_address, data_buf, data_len);
			//wait for completioin
			PT_WAIT(!buf);
			//check result: completed or error
			if(len){
				w2_master_software_reset(); //if error occured in reading, reset w2 according to nordic sdk
				ret=-EIO;
				goto outr;
			}
		}
		
	PT_E(outr)
}

PT(w2_master_read_reg, uint8_t slave_address, uint8_t regaddr,  uint8_t * data_buf, uint16_t data_len)
{
	PT_B()
		//wait for completion
		PT_WAIT(!buf);
				
		//Send the start condition, control byte, and address byte(s) in a write operation
		w2_master_io(W2_MASTER_WRITE, slave_address, &regaddr, 1);
		
		//wait for completion
		PT_WAIT(!buf);
		//check result: completed or error
		CHECKOUT(len,-EIO,outr);
	
		PT_CALL(w2_master_read,slave_address,data_buf,data_len);
		
	PT_E(outr)
}
	
PT(w2_master_write,uint8_t slave_address, uint8_t * data_buf, uint16_t data_len)
{

	PT_B()
		//wait for completion
		PT_WAIT(!buf);
		
		if(data_buf){
			//Send the writing request
			w2_master_io(W2_MASTER_WRITE,slave_address, data_buf, data_len);
			//wait for completioin
			PT_WAIT(!buf);
			//check result: completed or error
			CHECKOUT(len,-EIO,outw);
		}
		
	PT_E(outw)
}

PT(w2_master_write_reg,uint8_t slave_address, uint8_t regaddr, uint8_t * data_buf, uint16_t data_len)
{

	PT_B()
		//wait for completion
		PT_WAIT(!buf);
		
		//Send the writing request
		w2_master_io(W2_MASTER_WRITE,slave_address, &regaddr, 1);
		//wait for completioin
		PT_WAIT(!buf);
		//check result: completed or error
		CHECKOUT(len,-EIO,outw);
		
		PT_CALL(w2_master_write,slave_address,data_buf,data_len);
		
	PT_E(outw)
}

