#include "mspi.h"
#include "interrupt.h"
#include "w2.h"

#define MSPI_TX_FIFO_READY_INT_ENABLE				(0)																					//Enable TX FIFO ready interrupt
#define MSPI_TX_FIFO_READY_INT_DISABLE			((SPIMCON1_INT_TX_FIFO_READY_DISABLE) )	//Disable TX FIFO ready interrupt

#define MSPI_TX_FIFO_EMPTY_INT_ENABLE				(0)																					//Enable TX FIFO empty interrupt
#define MSPI_TX_FIFO_EMPTY_INT_DISABLE			((SPIMCON1_INT_TX_FIFO_EMPTY_DISABLE) )	//Disable TX FIFO empty interrupt

#define MSPI_RX_DATA_READY_INT_ENABLE				(0)																					//Enable RX data ready interrupt
#define MSPI_RX_DATA_READY_INT_DISABLE			((SPIMCON1_INT_RX_DATA_READY_DISABLE) )	//Disable RX data ready interrupt

#define MSPI_RX_FIFO_FULL_INT_ENABLE				(0)																					//Enable RX FIFO full interrupt
#define MSPI_RX_FIFO_FULL_INT_DISABLE				((SPIMCON1_INT_RX_DATA_FULL_DISABLE) )	//Disable RX FIFO full interrupt

#define	mspi_send(data_to_send)		(SPIMDAT = (data_to_send))										//Sends data_to_send over MSPI
#define	mspi_get()					(SPIMDAT)														//Returns last data received over MSPI
#define	mspi_is_rx_fifo_full()		(SPIMSTAT & SPIMSTAT_INT_RX_DATA_FULL_FLAG)		//True if RX FIFO is full, false otherwise
#define	mspi_is_rx_data_ready()		(SPIMSTAT & SPIMSTAT_INT_RX_DATA_READY_FLAG)	//True if RX data is ready, false otherwise
#define	mspi_is_tx_fifo_empty()		(SPIMSTAT & SPIMSTAT_INT_TX_FIFO_EMPTY_FLAG)	//True if TX FIFO is empty, false otherwise
#define	mspi_is_tx_fifo_ready()		(SPIMSTAT & SPIMSTAT_INT_TX_FIFO_READY_FLAG)	//True if TX FIFO is ready, false otherwise

#define	MSPI_SPIMCON0_MASK		0x7F	//Mask for the options used for SPIMCON0
#define MSPI_SPIMCON1_MASK		0x0F	//Mask for the options used for SPIMCON1


static volatile __pdata uint8_t * __data buf=0;
static volatile __data uint16_t len=0;
static __bit busy=0;

void mspi_init(uint8_t mspi_config_options){
	//Set up SPIMCON0 from mspi_config_options
	SPIMCON0 = (SPIMCON0 & ~MSPI_SPIMCON0_MASK) | ((uint8_t)(mspi_config_options) & MSPI_SPIMCON0_MASK);
	//only enable the rx data ready interrupt
	SPIMCON1 = (SPIMCON1 & ~MSPI_SPIMCON1_MASK) | ((MSPI_TX_FIFO_READY_INT_DISABLE|MSPI_TX_FIFO_EMPTY_INT_DISABLE|MSPI_RX_FIFO_FULL_INT_DISABLE) & MSPI_SPIMCON1_MASK);
	buf=0;
	len=0;
	//make sure the RX FIFO is clear 
	while(mspi_is_rx_data_ready())mspi_get();
	
	irq_flag_clear(irq_flag_spi_2wire); // Is this neccesary?
	irq_spi_2wire_enable();
}

void mspi_irq_handle(void) __reentrant {
	if(mspi_is_rx_data_ready()&&len){
		*buf++=mspi_get();
		if(--len)mspi_send(*buf);
		else buf=0;
	}
}

irq_isr_spi_2wire(){
	mspi_irq_handle();
	w2_irq_handle();
}

static void mspi_exchange(__pdata void * mspi_data, uint16_t size){
	if(mspi_data&&size){
		buf=mspi_data;
		len=size;
		//send the byte
		mspi_send(*buf);
	}
}

PT(mspi_io, __pdata void * data, uint16_t size){
	PT_B
		PT_LOCK(busy);
		mspi_exchange(data,size);
		PT_WAIT(!buf);
		PT_UNLOCK(busy);
	PT_E
}
