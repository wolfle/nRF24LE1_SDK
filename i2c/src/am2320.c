#include "w2.h"
#include "am2320.h"
#include "timer.h"


#define ADDRESS 0x5c

#define READ 0x03
#define WRITE 0x10

#define CRCLEN 6

static __pdata const uint8_t tbuf[3]={0x03,0x00,0x04};
static __pdata uint8_t rbuf[8];
static __pdata uint16_t jiffers0,jiffers1;

static uint16_t crc(void){
	uint16_t crc=0xffff;
	uint8_t i=0,ii;
	for(;i<CRCLEN;++i){
		crc^=rbuf[i];
		for(ii=0;ii<8;++ii){
			if(crc & 0x1){
				crc>>=1;
				crc^=0xa001;
			}else crc>>=1;
		}
	}
	return crc;
}

PT(am2320_read,__pdata uint32_t * res){
	uint16_t cr;
	PT_B
	PT_CALL(w2_master_write,ADDRESS,tbuf, sizeof(tbuf)); //first write should fail and wake up am2320.
	PT_CALL_OUT(out,w2_master_write,ADDRESS,tbuf, sizeof(tbuf));
	PT_CALL_OUT(out,w2_master_read,ADDRESS,rbuf,sizeof(rbuf));
	PT_CALL(delay_dms,20000,&jiffers0,&jiffers1);//delay 2s;
	PT_CALL_OUT(out,w2_master_write,ADDRESS,tbuf, sizeof(tbuf));
	PT_CALL_OUT(out,w2_master_read,ADDRESS,rbuf,sizeof(rbuf));
	CHECKOUT(rbuf[0]!=0x03 || rbuf[1] != 0x04,-EINVAL,out);
	cr=crc();
	CHECKOUT(cr != rbuf[6] + (rbuf[7]<<8), -EINVAL,out);
	//sdcc is little endian, ie LS byte first, but the results are big endian, ie MSB first
	*((uint8_t *)res)=rbuf[5];
	((uint8_t *)res)[1]=rbuf[4];
	((uint8_t *)res)[2]=rbuf[3];
	((uint8_t *)res)[3]=rbuf[2];
	
/*
	*res=rbuf[2];
	*res<<=8;
	*res+=rbuf[3];
	*res<<=8;
	*res+=rbuf[4];
	*res<<=8;
	*res+=rbuf[5];
*/
out:
	PT_E
}
