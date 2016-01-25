#include "memory.h"

int16_t flash_get_page_size(uint8_t page){
	return page < FLASH_CODE_NUM_PAGES ? FLASH_CODE_PAGE_SIZE:
		(page < (FLASH_CODE_NUM_PAGES + FLASH_NV_EXT_NUM_PAGES)) ? FLASH_NV_EXT_PAGE_SIZE:
		page < FLASH_NUM_PAGES ? FLASH_NV_STD_PAGE_SIZE:
		-1;
}

