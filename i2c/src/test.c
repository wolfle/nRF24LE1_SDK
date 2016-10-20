#include "ssd1306.h"
#include "bmp180.h"
#include "am2320.h"
#include "gp2y1051.h"
#include "timer.h"
#include "rf.h"
#include "w2.h"

/* RF channels searched for devices */
#define CHANNEL 2
/* 5 byte prearranged pipe address */
#define PIPE_ADDRESS 0xBA,0xDA,0x55,0x13,0x37


static const uint8_t default_pipe_address[5] = {PIPE_ADDRESS};

struct results{
	uint32_t amres, bmpp;
	uint16_t gp2yv;
};

static const struct rf_params rfp={
	.addr=default_pipe_address,
	.addrlen=sizeof(default_pipe_address),
	.channel=CHANNEL,
	.retrans=5,
	.dr=dr2m,
	.pa=n0dbm
};

static __pdata struct results r;
static volatile __pdata uint32_t gp2ysum;
static volatile __pdata uint8_t gp2yno;
static volatile __bit gp2yflag=0, th0=1, th1=1, th2=1;

static lc_t bmp[5]={PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL},
			am[4]={PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL},
			gp[4]={PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL,PT_LC_INITVAL};

irq_isr_spi_2wire()__using(1){	w2_irq_handle(); 	}

static void gp2ycb(uint16_t v) __reentrant{
	gp2yflag=0;
	gp2ysum+=v;
	++gp2yno;
	gp2yflag=1;
}

static PT_V(ssd_init){//4
	PT_B
	PT_CALLV(ssd1306_clear); // buffer is full of entropy on boot
	PT_CALL(ssd1306_printf,0,0,"H              C");
	PT_CALL(ssd1306_printf,2,0,"P:            Pa");
	PT_CALL(ssd1306_printf,4,0,"Dust v:");
	PT_CALL(ssd1306_printf,6,0,"Rf f: ");
	PT_E;
}
	
static PT_V(devices_init){//5
	PT_B
	PT_CALLV(ssd1306_init);
	PT_CALLV(bmp180_init);
	PT_CALLV(ssd_init);
	PT_E
}	

static PT_V(am2320){//4
	PT_B
	PT_CALL(am2320_read,&r.amres);
	PT_CALL(ssd1306_printf,0,2,"%u%%%% T %u",r.amres>>16,r.amres&0xffff);
	PT_E
}

static PT_V(bmp180){ //4
	PT_B
	PT_CALLV(bmp180_measure_t);
	PT_CALL(bmp180_measure_p,BMP180_OSS_DOUBLE,&r.bmpp);
	PT_CALL(ssd1306_printf,2,3,"%u",r.bmpp);
	PT_E
}

static PT_V(gp2y){//4
	PT_B
	PT_WAIT(gp2yflag);
	PT_CALL(ssd1306_printf,4,8, "%u", gp2ysum/gp2yno);
	PT_E
}

static PT_V(send){ //2
	PT_B
	PT_WAIT(gp2yflag);
	r.gp2yv=gp2ysum/gp2yno;
	PT_CALL(rf_send,&r, sizeof(r));
	PT_CALL(ssd1306_printf,6,6,"%u s: %u",rf_lost(),rf_sent());
	PT_E
}

void run(void){
	while(th0||th1||th2){
		if(th1 && bmp180(bmp)<PT_WAITING)th1=0;
		if(th2 && am2320(am)<PT_WAITING)th2=0;
		if(th0 && gp2y(gp)<PT_WAITING)th0=0;
	}
	PT_INIT(bmp[0]);
	PT_INIT(am[0]);
	PT_INIT(gp[0]);
	while(send(bmp)>=PT_WAITING);
	PT_INIT(bmp[0]);
}

char main(void){
		
	dms_init();
	w2_init(W2_ENABLE|W2_MASTER|W2_FREQ_400_KHZ,0);
	rf_init(&rfp,false,true);
	
	while(devices_init(bmp)>=PT_WAITING);
	
	gp2y_init(gp2ycb);

	run();
		
//	ssd1306_displayoff();
	
	return 0;
}

