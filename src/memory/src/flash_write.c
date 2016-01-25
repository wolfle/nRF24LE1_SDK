//#include "memory.h"
#include "interrupt.h"

//macros
#define flash_write_enable()				sbit_set(FSR_SB_WEN)
#define flash_write_disable()				sbit_clear(FSR_SB_WEN)
#define wait_for_flash_write_complete()			while(FSR_SB_RDYN)
#define erase_flash_page(num)						FCR=num

#define flash_write_begin	cli_save(PSW_SB_F0);flash_write_enable();
	
#define flash_write_end	    flash_write_disable();irq_restore_global_flag(PSW_SB_F0);

#define flash_write_op(x) flash_write_begin {x} wait_for_flash_write_complete(); flash_write_end
#define flash_write_opnw(x) flash_write_begin {x} flash_write_end

void flash_erase_page(uint8_t page){
	flash_write_op(	erase_flash_page(page); )
}

void flash_write_byte(uint16_t addr, uint8_t val){
	flash_write_op( *((__xdata uint8_t *)addr) = val; )
}

void flash_write_bytes(uint16_t addr, uint8_t * src, uint16_t len)
{
	flash_write_opnw(
		while(len--) {
			*(__xdata uint8_t *)addr++ = *src++;
			wait_for_flash_write_complete();
		} )
}

