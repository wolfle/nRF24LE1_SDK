#include "../i2c.h"
#include <stdint.h>
#include <time.h>

int iic_read_a(uint8_t addr, uint8_t regaddr, uint8_t length, uint8_t *data) {
    return w2_master_read(fd,addr,regaddr,data,length);
}


int iic_write_a(uint8_t addr, uint8_t regaddr, uint8_t length, uint8_t* data) {
    return iic_write_reg(fd,addr,regaddr,data,length);
}

void getclockms(unsigned long * tms){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC,&t);
	*tms=t.tv_sec*1000+t.tv_nsec/1000000;
}
