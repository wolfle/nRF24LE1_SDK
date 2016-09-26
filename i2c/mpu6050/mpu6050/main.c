#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "mpu/sensor.h"
#include "mpu/iica/iica.h"
#include "mpu/qmath.h"

#define delay_ms(a) usleep(a*1000)

uint8_t rate = 40;

int main() {
	int res;
	struct Mpu m;
	float rpy[3];
	PRERR(res,iic_open())
	PRERR(res,md_open(&m,2000,2,rate))
	
//	int tsp=0;
	
	do{
//		if(md_update(&m)>0 && tsp<1000/rate-5)++tsp;
		md_update(&m);
		q2Euler(rpy,m.nq);
//		printf("Q: %li %li %li %li  R: %2.1f P: %2.1f\n",m.q[0],m.q[1],m.q[2],m.q[3],m.rpy[0],m.rpy[1]);
		printf("R: %2.1f  P: %2.1f  Y: %2.1f  G: %2.1f %2.1f %2.1f  A: %2.1f %2.1f %2.1f\n",
			m.rpy[ROLL], m.rpy[PITCH], m.rpy[YAW],
			m.gyro[0],m.gyro[1],m.gyro[2],
			m.accel[0],m.accel[1],m.accel[2]);
//		delay_ms(1000/rate-tsp);
		delay_ms(15);
	}while(1);
	
	iic_close();
	return 0;
}
