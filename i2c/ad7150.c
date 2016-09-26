#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "i2c.h"

#define ADDRESS 0x48

#define STATUS              0
#define STATUS_OUT1         (1 << 3)
#define STATUS_OUT2         (1 << 5)
#define CH1_DATA_HIGH       1
#define CH2_DATA_HIGH       3
#define CH1_AVG_HIGH        5
#define CH2_AVG_HIGH        7
#define CH1_SENSITIVITY     9
#define CH1_THR_HOLD_H      9
#define CH1_TIMEOUT         10
#define CH1_SETUP           11
#define CH2_SENSITIVITY     12
#define CH2_THR_HOLD_H      12
#define CH2_TIMEOUT         13
#define CH2_SETUP           14
#define CFG                 15
#define CFG_FIX             (1 << 7)
#define PD_TIMER            16
#define CH1_CAPDAC          17
#define CH2_CAPDAC          18
#define SN3                 19
#define SN2                 20
#define SN1                 21
#define SN0                 22
#define ID                  23

static const unsigned char addresses[][6] = {
	{ CH1_DATA_HIGH, CH1_AVG_HIGH,
	  CH1_SETUP, CH1_THR_HOLD_H,
	  CH1_SENSITIVITY, CH1_CAPDAC },
	{ CH2_DATA_HIGH, CH2_AVG_HIGH,
	  CH2_SETUP, CH2_THR_HOLD_H,
	  CH2_SENSITIVITY, CH2_CAPDAC },
};

static const float range[]={2,0.5,1,4};

static inline float capacity(char buf[], unsigned int channel){
	return (buf[STATUS]&(channel+1)) ? NAN:
		   (buf[addresses[channel][0]]<0x30)?-1:
		   (buf[addresses[channel][0]]>=0xD0)?INFINITY:
		   range[buf[addresses[channel][2]]>>6] 
				*((buf[addresses[channel][0]]<<8)+buf[addresses[channel][0]+1]
					-0x3000)/0x9ff0
		   +10.0*(buf[addresses[channel][5]]&0x3f)/0x3f;
}

int main(void){
  // Set up some variables that we'll use along the way
  char buf[24];  // receive buffer
 
  int res,ibus = iic_open_bus("/dev/i2c-2"); //ad7150 address = 0x48 
  if(ibus<0)return ibus;
  
  // Clear our buffers
  memset(buf, 0, sizeof(buf));
  
  char tx = 0x00; // This is the start address we want to read from.
  if(res=iic_write(ibus, ADDRESS,&tx, 1))return res;
  if(res=iic_read(ibus, ADDRESS,buf, 24))return res;
  printf("status, config : %#x, %#x\n", buf[STATUS],buf[CFG]); // should print 0x54 or 0x53 or 0x52 or 0x51 or 0x50

#if 0  
  printf("range1,range2: %#x, %#x\n",buf[CH1_SETUP]>>6,
								buf[CH2_SETUP]>>6);
								
  printf("CAPDAC ch1, ch2: %#x, %#x\n",buf[CH1_CAPDAC],buf[CH2_CAPDAC]);
  {
	  int ch1=buf[CH1_DATA_HIGH],
					 ch2=buf[CH2_DATA_HIGH]; 
	  ch1<<=8;ch1+=buf[CH1_DATA_HIGH+1];
	  ch2<<=8;ch2+=buf[CH2_DATA_HIGH+1];
									
	  float c1=range[buf[CH1_SETUP]>>6]*(ch1-0x3000)/0x9ff0+10.0*(buf[CH1_CAPDAC]&0x3f)/0x3f,
			c2=range[buf[CH2_SETUP]>>6]*(ch2-0x3000)/0x9ff0+10.0*(buf[CH2_CAPDAC]&0x3f)/0x3f;
	  printf("ch1,ch2,c1,c2 ; c1,c2: %#x, %#x, %f, %f ; %f, %f\n",ch1,ch2,c1,c2,capacity(buf,0),capacity(buf,1));
  }
  printf("id : %#x\n",buf[ID]);
#endif
  
  while(1){
	if(res=iic_write(ibus, ADDRESS,&tx, 1))return res;
	if(res=iic_read(ibus, ADDRESS,buf, 19))return res;	
//	printf("status %#x ; ",buf[STATUS]); 
	if((buf[STATUS]&0xF0) < 0x80)
		printf("Channel 1: %6.3f ; Channel 2: %6.3f\n",
				capacity(buf,0),capacity(buf,1));
	usleep(200000); //0.2 second
  }
  iic_close_bus(ibus);
  return 0; 
 }
