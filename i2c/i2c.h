#ifndef I2C_H_
#define I2C_H_

#include "w2.h"

static inline int iic_write(uint8_t addr, void * buf, unsigned short len){
  // Tell the I2C peripheral that the device address is (or isn't) a 10-bit
//	if(addr > 0xFF) ioctl(fh, I2C_TENBIT, 1);//FIXME: is this right?
	return ioctl(fd, I2C_SLAVE, addr)?ERROR_IIC_SET_ADDRESS:
			write(fd, buf, len)!=(int)len?ERROR_IIC_WRITE:0;
}

static inline int iic_read(uint8_t addr, void * buf, unsigned short len){
  // Tell the I2C peripheral that the device address is (or isn't) a 10-bit
//	if(addr > 0xFF) ioctl(fh, I2C_TENBIT, 1);//FIXME: is this right?
	return ioctl(fd, I2C_SLAVE, addr)?ERROR_IIC_SET_ADDRESS:
			read(fd, buf, len)!=(int)len?ERROR_IIC_READ:0;
}

static inline int iic_read_reg(uint8_t addr, uint8_t reg,void * buf, unsigned short len){
	int res;
    PRERR(res,iic_write(fd, addr, &reg, 1))
    return iic_read(fd, addr, buf, len);
}

static inline int iic_write_reg(uint8_t addr, unsigned char reg,void * buf, unsigned short len){
	unsigned char buf0[len+1];
    buf0[0] = reg;
    memcpy(buf0+1,buf,len);
    return iic_write(fd,addr,buf0,sizeof(buf0));
}

#endif
