//#include "memory.h"
#include <stdint.h>

void flash_read_bytes(uint16_t addr, uint8_t * dest, uint16_t len){
  while(len--) *dest++ = *(uint8_t __xdata *)addr++;
}

