/////////////////////////////////////////////////////////////////////////////
//
// File: memory.h
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

#ifndef MEMORY_H_
#define MEMORY_H_

#include "reg24le1.h"

//////////////////////////////////////////////////////////
// Defines for the code section of the flash memory array
//////////////////////////////////////////////////////////
#define	FLASH_CODE_NUM_PAGES			(32)
#define FLASH_CODE_FIRST_PAGE_NUM	(0)
#define FLASH_CODE_LAST_PAGE_NUM		(FLASH_CODE_FIRST_PAGE_NUM + FLASH_CODE_NUM_PAGES - 1)
#define	FLASH_CODE_PAGE_SIZE			(512)
#define FLASH_CODE_NUM_BYTES			(FLASH_CODE_NUM_PAGES * FLASH_CODE_PAGE_SIZE)
#define FLASH_CODE_START_ADDRESS		(0)
#define FLASH_CODE_END_ADDRESS		(FLASH_CODE_START_ADDRESS + FLASH_CODE_NUM_BYTES - 1)

//////////////////////////////////////////////////////////////////////////////
// Defines for the extended endurance NVRAM section of the flash memory array
//////////////////////////////////////////////////////////////////////////////
#define	FLASH_NV_EXT_NUM_PAGES		(2)
#define FLASH_NV_EXT_FIRST_PAGE_NUM	(FLASH_CODE_LAST_PAGE_NUM + 1)
#define FLASH_NV_EXT_LAST_PAGE_NUM	(FLASH_NV_EXT_FIRST_PAGE_NUM + FLASH_NV_EXT_NUM_PAGES - 1)
#define	FLASH_NV_EXT_PAGE_SIZE		(256)
#define FLASH_NV_EXT_NUM_BYTES		(FLASH_NV_EXT_NUM_PAGES * FLASH_NV_EXT_PAGE_SIZE)
#define FLASH_NV_EXT_START_ADDRESS	(0xFA00)
#define FLASH_NV_EXT_END_ADDRESS		(FLASH_NV_EXT_START_ADDRESS + FLASH_NV_EXT_NUM_BYTES - 1)

//////////////////////////////////////////////////////////////////////////////
// Defines for the extended endurance NVRAM section of the flash memory array
//////////////////////////////////////////////////////////////////////////////
#define	FLASH_NV_STD_NUM_PAGES		(2)
#define FLASH_NV_STD_FIRST_PAGE_NUM	(FLASH_NV_EXT_LAST_PAGE_NUM + 1)
#define FLASH_NV_STD_LAST_PAGE_NUM	(FLASH_NV_STD_FIRST_PAGE_NUM + FLASH_NV_STD_NUM_PAGES - 1)
#define	FLASH_NV_STD_PAGE_SIZE		(512)
#define FLASH_NV_STD_NUM_BYTES		(FLASH_NV_STD_NUM_PAGES * FLASH_NV_STD_PAGE_SIZE)
#define FLASH_NV_STD_START_ADDRESS	(0xFC00)
#define FLASH_NV_STD_END_ADDRESS		(FLASH_NV_STD_START_ADDRESS + FLASH_NV_STD_NUM_BYTES - 1)

//////////////////////////////////////////////
// Defines for the overall flash memory array
//////////////////////////////////////////////
#define	FLASH_NUM_PAGES					(FLASH_CODE_NUM_PAGES + FLASH_NV_EXT_NUM_PAGES + FLASH_NV_STD_NUM_PAGES)


/////////////////////////////////////////////////
// Function macros for general memory operations
/////////////////////////////////////////////////
#define movx_access_data()	(PCON &= (~PCON_PMW))
#define movx_access_code()	(PCON |= PCON_PMW)

///////////////////////////////////////
// Function macros for SRAM operations
///////////////////////////////////////
#define map_retentive_sram_as_data()		(MEMCON &= (~MEMCON_SRAM_DATA_RETENTIVE_MAPPING))
#define map_retentive_sram_as_code()		(MEMCON |= MEMCON_SRAM_DATA_RETENTIVE_MAPPING)
#define map_non_retentive_sram_as_data()	(MEMCON &= (~MEMCON_SRAM_DATA_NON_RETENTIVE_MAPPING))
#define map_non_retentive_sram_as_code()	(MEMCON |= MEMCON_SRAM_DATA_NON_RETENTIVE_MAPPING)
#define sram_start_at_0x0000()					(MEMCON &= (~MEMCON_SRAM_START_LOCATION))
#define sram_start_at_0x8000()					(MEMCON |= MEMCON_SRAM_START_LOCATION)

////////////////////////////
// Function prototypes
////////////////////////////
void flash_read_bytes(uint16_t addr, uint8_t * dest, uint16_t len);
void flash_erase_page(uint8_t page);
void flash_write_byte(uint16_t addr, uint8_t val);
void flash_write_bytes(uint16_t addr, uint8_t * src, uint16_t len);
int16_t flash_get_page_size(uint8_t page); //return -1 if addr not in range
char flash_get_page_num(uint16_t addr); //return -1 if addr not in range
	
inline uint8_t flash_read_byte(uint16_t addr){return *((uint8_t __xdata *)addr);}
//Determine if the new val can be written into the flash with cur val without page erasion. Write can only do 1->0. Erasion make all bits to 1.
inline bool flash_can_be_written_without_erase(uint8_t cur, uint8_t newval){	return ! ( (cur ^ newval) & newval );}

inline bool is_address_in_flash(uint16_t addr){return addr <= FLASH_CODE_END_ADDRESS;}
inline bool is_address_in_nv_ext(uint16_t addr){return (addr >= FLASH_NV_EXT_START_ADDRESS) && (addr <= FLASH_NV_EXT_END_ADDRESS);}
inline bool is_address_in_nv_std(uint16_t addr){return (addr >= FLASH_NV_STD_START_ADDRESS);}

#endif /* MEMORY_H_ */
