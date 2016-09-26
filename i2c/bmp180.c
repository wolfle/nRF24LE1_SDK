#include <stdint.h>
#include "w2.h"
#include "error.h"
#include "bmp180.h"
#include "timer.h"

#define ADDRESS 0x77

#define CALIBREG 0xAA
#define CALIBLEN 22

#define XLSB 0xF8
#define LSB 0xF7
#define MSB 0xF6

#define CTR 0xF4
#define RST 0xE0

#define IDREG 0xD0
#define IDV 0x55

#define TEMPM 0x2E  //wait 4.5ms
#define PRESM 0x34	//wait {4.5,7.5,13.5,25.5} for oversampling {0,1,2,3}

const static uint8_t delays[]={45,75,135,255}; //*0.1ms

//Msb first in the regs

static struct Bmp180{
	union{
		struct {
			int16_t ac1,ac2,ac3;
			uint16_t ac4,ac5,ac6;
			int16_t b1,b2,mb,mc,md;
		};
		uint8_t cbuf[CALIBLEN];
	};
	int32_t b5;
	uint8_t oss,buf[3];
} b;

static uint16_t jiffers0, jiffers1;
static __bit busy=0; //act as a lock of bmp180
static PT_DECLARE(lc);

static uint32_t calib_t(void){
	int32_t temp=((uint16_t)b.buf[0]<<8)+b.buf[1];
	temp=((temp-b.ac6)*b.ac5) >> 15; //X1
	b.b5=temp + (b.mc*(1<<11))/(temp+b.md);
	return (b.b5+8)>>4;
}

static int32_t calib_p(void){
	int32_t b6=b.b5-4000, b3, x, p=((uint32_t)b.buf[0]<<16)+((uint16_t)b.buf[1]<<8)+b.buf[2];
	uint32_t b4,b7;
	x=(b.b2*((b6*b6)>>12))>>11;
	x+=(b.ac2*b6)>>11;
	b3=((((b.ac1<<2)+x)<<b.oss)+2)>>2;
	x=(b.b1*((b6*b6)>>12))>>16;
	x=(x+((b.ac3*b6)>>13)+2)>>2;
	b4=(b.ac4*((uint32_t)(x+32768)))>>15;
	b7=(uint32_t)((p>>(8-b.oss))-b3)*(50000>>b.oss);
	if(b7<0x80000000)p=(b7<<1)/b4;
	else p=(b7/b4)<<1;
	x=p>>8;
	x*=x;
	x=(x*3038)>>16;
	x+=3791+((-7357*p)>>16);
	p+=x>>4;
	return p;
}

PT_V(bmp180_init){
	char i=0;
	uint8_t temp;
	PT_B()
		PT_LOCK(busy);
		PT_CALL_OUT(out0,w2_master_read_reg,ADDRESS,IDREG,b.buf,1);
		CHECKOUT(b.buf[0]!=IDV,-ENODEV,out0);
		PT_CALL_OUT(out0,w2_master_read_reg,ADDRESS,CALIBREG,b.cbuf,sizeof(b.cbuf));
	//calib params are little endian, have to be swapped to big endian for the 8051 mcu
		for(;i<CALIBLEN;++i,++i){
			temp=b.cbuf[i];
			b.cbuf[i]=b.cbuf[i+1];
			b.cbuf[i+1]=temp;
		}
out0:	PT_UNLOCK(busy);
	PT_E(outi)
}

PT(bmp180_measure_t,int32_t * t){
	PT_B()
		PT_LOCK(busy);
		b.buf[0]=CTR;
		b.buf[1]=TEMPM;
		PT_CALL_OUT(out1,w2_master_write,ADDRESS,b.buf,2);
		
		PT_CALL(delay_dms,delays[0],&jiffers0,&jiffers1);
		
		PT_CALL_OUT(out1,w2_master_read_reg,ADDRESS,MSB,b.buf,sizeof(b.buf));
		*t=calib_t();
out1:	PT_UNLOCK(busy);
	PT_E(outt)
}

PT(bmp180_measure_p,uint8_t oss,int32_t *p){
	PT_B()
		PT_LOCK(busy);
		b.oss=oss&0x03;
		b.buf[0]=CTR;
		b.buf[1]=PRESM+(b.oss<<6);
		PT_CALL_OUT(out2,w2_master_write,ADDRESS,b.buf,2);

		PT_CALL(delay_dms,delays[b.oss],&jiffers0,&jiffers1);

		PT_CALL_OUT(out2,w2_master_read_reg,ADDRESS,MSB,b.buf,sizeof(b.buf));	
		*p=calib_p();
out2:	PT_UNLOCK(busy);
	PT_E(outp)	
}

/*
uint16_t bmp180_altitude(uint32_t p){ //return unit dm
	double p1=p,p0=101325;
	return 44330.0*(1.0-pow((p1/p0),(1.0/5.255)))*10.0;
}
*/
