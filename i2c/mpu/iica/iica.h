#ifndef IICA_H_
#define IICA_H_

#include "w2.h"

static inline int iic_read_a(uint8_t addr, uint8_t regaddr, uint8_t length, uint8_t *data);

static inline int iic_write_a(uint8_t addr, uint8_t regaddr, uint8_t length, uint8_t *data);

void getclockms(unsigned long * tms);

#endif
