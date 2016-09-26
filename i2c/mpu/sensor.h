#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

#define YAW 2
#define PITCH 1
#define ROLL 0
#define DIM 3

struct Mpu {
	int16_t a[3];              // [x, y, z]            accel vector
	int16_t g[3];              // [x, y, z]            gyro vector
	long q[4];		//Quaternion
//	long t;			//temperature, >>16 convert to float
//	int16_t c[3];	//for compass
	unsigned long timestamp;
	
	int gfsr;
	int afsr;
	uint8_t rate;
	
	float gsens;
	float asens;
	
	float accel[3];              // [x, y, z]            accel vector
	float gyro[3];              // [x, y, z]            gyro vector
	float nq[4];    //normalized quaternion
	float rpy[3];	//Roll pitch yaw angles	
};

int md_open(struct Mpu *m,uint16_t gfsr, uint8_t afsr, uint8_t rate);
int md_update(struct Mpu *m);

#endif
