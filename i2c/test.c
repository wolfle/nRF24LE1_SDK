#include "i2c.h"
#include "ssd1306.h"
#include "bmp180.h"
#include "am2320.h"
#include "mpu/sensor.h"
#include <unistd.h>
#include <stdio.h>

int main(void){
	int res,fd=iic_open_bus("/dev/i2c-2");
	if(fd<0){
		printf("Error open iic bus %s\n","/dev/i2c-2");
		return -1;
	}
	
	struct Mpu m;
	iic_open(fd);
	PRERR(res,md_open(&m,2000,2,42));

	struct Bmp180 b;
	if(res=bmp180_open(&b,fd)){
		printf("Error open  bmp180.\n");
		return -1;
	}
	
	ssd1306_init(fd);
	ssd1306_clear(); // buffer is full of entropy on boot

	if(res=bmp180_measure_t(&b)){
		printf("Error bmp180 issue measure t.\n");
		return -1;
	}
		
	usleep(bmp180_delay_t()*100);
	if((res=bmp180_result(&b))<0){
		printf("Error get bmp180 t result %i.\n",res);
		return -1;
	}else printf("Temperature: %u\n", (uint32_t)res);

	if(res=bmp180_measure_p(&b,OSS_DOUBLE)){
		printf("Error bmp180 issue measure p.\n");
		return -1;
	}
	
	usleep(bmp180_delay_p(&b)*100);
	if((res=bmp180_result(&b))<0){
		printf("Error get bmp180 p result %i.\n",res);
		return -1;
	}else{
		ssd1306_printf(2,"P: %u Pa",res);
		ssd1306_printf(3,"Alt: %.1f m",bmp180_altitude(res)*0.1);
	}

	res=am2320_read(fd); //read out stale values
	sleep(am2320_delay()); //wait 2s for new measurement
	res=am2320_read(fd); //read out current values
	
	if(res<0)printf("Error in am_read: %i\n",res);
	else{
		ssd1306_printf(4,"R Hum: %.1f%%",0.1*(res>>16));
		ssd1306_printf(5,"T: %.1f C",0.1*(res&0xffff));
	}
	
	do{
		md_update(&m);
		ssd1306_printf(0, "E %2.1f %2.1f %2.1f", m.rpy[0],m.rpy[1],m.rpy[2]);
		ssd1306_printf(1, "G %2.1f %2.1f %2.1f", m.gyro[0],m.gyro[1],m.gyro[2]);
		usleep(15*1000);
	}while(1);
	
//	ssd1306_displayoff();
	
		
	iic_close_bus(fd);
	return 0;
}

