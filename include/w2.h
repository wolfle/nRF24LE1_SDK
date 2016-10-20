#ifndef _W2_H_
#define _W2_H_

#include <stdint.h>
#include "pt.h"

#define W2_DISABLE						0																					//Disable W2
#define W2_ENABLE						(1<<7)							//Enable W2

#define W2_SLAVE						0																					//Run as a slave
#define W2_MASTER						(1<<6)					//Run as a master

#define W2_FREQ_IDLE					0x00 	//Keep clock idle
#define W2_FREQ_100_KHZ					0x01 	//Run clock at 100 kHz (requires system clock of >= 4 MHz)
#define W2_FREQ_400_KHZ					0x02 	//Run clock at 400 kHz (requires system clock of >= 8 MHz)

#define W2_SIRQ_ADDR_MATCH_ENABLE		0																					//For slaves, enable interrupt on address match
#define W2_SIRQ_ADDR_MATCH_DISABLE		(1<<5)	//For slaves, disable interrupt on address match

#define W2_SIRQ_STOP_ENABLE				0																					//For slaves, enable interrupt on reception of stop condition
#define W2_SIRQ_STOP_DISABLE			(1<<4)		//For slaves, disable interrupt on reception of stop condition

#define W2_SCLK_ACTIVE_BETWEEN_XFERS	0																					//For slaves, keep clock low between transmitted bytes (clock stretching)
#define W2_SCLK_LOW_BETWEEN_XFERS		(1<<3)				//For slaves, keep clock running between transmitted bytes

#define W2_SBROADCAST_DISABLE			0																					//For slaves, do not respond to broadcast address (0x00)
#define W2_SBROADCAST_ENABLE			(1<<2)			//For slaves, respond to broadcast address (0x00)

////////////////////////////
// Function macros
////////////////////////////
#define w2_enable()						W2CON0 |= W2CON0_ENABLE																			//Enable W2
#define w2_disable()					W2CON0 &= (~W2CON0_ENABLE)																		//Disable W2


////////////////////////////
// Function prototypes
////////////////////////////
void w2_init(uint8_t w2_config_options, uint8_t address_in_slave_mode);
void w2_irq_handle(void);
PT(w2_master_write, uint8_t slave_address, __pdata void *  data_buf, uint16_t data_len); //1
PT(w2_master_read, uint8_t slave_address, __pdata void * data_buf, uint16_t data_len);  //1
PT(w2_master_read_reg, uint8_t slave_address,  uint8_t regaddr,  __pdata void * data_buf, uint16_t data_len); //2
PT(w2_master_write_reg, uint8_t slave_address, uint8_t regaddr, __pdata void * data_buf, uint16_t data_len);//2

#endif /* _W2_H_ */
