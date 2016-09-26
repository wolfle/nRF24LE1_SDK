#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "inv_mpu_lib/inv_mpu.h"
#include "inv_mpu_lib/inv_mpu_dmp_motion_driver.h"
#include "sensor.h"
#include "iica/iica.h"
#include "qmath.h"

#define SHIFT 16

#define delay_ms(a)    usleep(a*1000)

//#define FSR 2000
//#define GYRO_SENS       16.375f 
//#define QUAT_SENS       1073741824.f //2^30

static inline float gyrosens(int fsr){
	return ( 131.0f * 250.f / (float)fsr );
}

static inline float accelsens(int fsr){
	return (float)((1<<15)-1)/(float)fsr/9.8;
}

static inline float rad2deg( float rad ){ //return (180.f/PI) * rad;
	return 57.2957795131f * rad;
}

static inline float wrap_180(float x) {
	return (x<-180.f?x+360.f:(x>180.f?x-180.f:x));
}
/*
static void gravity(float *v, long *q) {
	v[0] = 2 * (q[1]*q[3] - q[0]*q[2]);
	v[1] = 2 * (q[0]*q[1] + q[2]*q[3]);
	v[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
}

static void rollpitchyaw(float *data, long *q) {
	float v[3];
	gravity(v,q);
	// yaw: (about Z axis)
	data[2] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1);
	// pitch: (nose up/down, about Y axis)
	data[1] = atan(v[0] / sqrt(v[1]*v[1] + v[2]*v[2]));
	// roll: (tilt left/right, about X axis)
	data[0] = atan(v[1]/ sqrt(v[0]*v[0] + v[2]*v[2]));
}
*/
int md_open(struct Mpu * m, uint16_t gfsr, uint8_t afsr, uint8_t rate) {
	uint8_t status;
	int res;
	// initialize device
	printf("Initializing MPU...\n");
	PRERR(res,mpu_init(NULL))
	// set gyro and accel fsr; set sensors; set sample rate; set fifo; set lpf
	printf("Setting MPU sensors...\n");
	PRERR(res,mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL))
	
	printf("Setting GYRO sensitivity...\n");
	PRERR(res,mpu_set_gyro_fsr(gfsr))
	
	printf("Setting ACCEL sensitivity...\n");
	PRERR(res,mpu_set_accel_fsr(afsr))
	// verify connection
	printf("Powering up MPU...\n");
	mpu_get_power_state(&status);
	printf(status ? "MPU6050 connection successful\n" : "MPU6050 connection failed %u\n",status);

	//fifo config
	printf("Setting MPU fifo...\n");
	PRERR(res,mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL))

	printf("Setting Lpf...\n");
	PRERR(res,mpu_set_lpf(rate))
	
	// load and configure the DMP
	printf("Loading DMP firmware...\n");
	PRERR(res,dmp_load_motion_driver_firmware())

	printf("Activating DMP...\n");
	PRERR(res,mpu_set_dmp_state(1))

	//dmp_set_orientation()
	//if (dmp_enable_feature(DMP_FEATURE_LP_QUAT|DMP_FEATURE_SEND_RAW_GYRO)!=0) {
	printf("Configuring DMP...\n");
/*
DMP_FEATURE_TAP
DMP_FEATURE_ANDROID_ORIENT
DMP_FEATURE_LP_QUAT
DMP_FEATURE_PEDOMETER
DMP_FEATURE_6X_LP_QUAT
DMP_FEATURE_GYRO_CAL
DMP_FEATURE_SEND_RAW_ACCEL
DMP_FEATURE_SEND_RAW_GYRO
DMP_FEATURE_SEND_CAL_GYRO
*/
	PRERR(res,dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|
						DMP_FEATURE_SEND_RAW_ACCEL|
						DMP_FEATURE_SEND_CAL_GYRO|
						DMP_FEATURE_GYRO_CAL))


	printf("Setting DMP fifo rate...\n");
	PRERR(res,dmp_set_fifo_rate(rate))
	
	printf("Resetting fifo queue...\n");
	PRERR(res,mpu_reset_fifo())

	m->gfsr=gfsr;
	m->afsr=afsr;
	m->rate=rate;
	
	m->gsens=gyrosens(m->gfsr);
	m->asens=accelsens(m->afsr);

	return 0;
}

int md_update(struct Mpu * m) {
	unsigned long timestamp;
	uint8_t more;
	int16_t sensors;	//data flags
	
	while (dmp_read_fifo(m->g,m->a,m->q,&timestamp,&sensors,&more)); //gyro and accel can be null because of being disabled in the efeatures
//	mpu_get_temperature(&(m->t),&timestamp);
	
//	mpu_get_compass_reg(m->c,&(m->timestamp));

//INV_XYZ_GYRO   INV_XYZ_ACCEL   INV_WXYZ_QUAT
	if(sensors&INV_WXYZ_QUAT){
		
		m->q[0]>>=SHIFT;
		m->q[1]>>=SHIFT;
		m->q[2]>>=SHIFT;
		m->q[3]>>=SHIFT;
		m->t>>=SHIFT;
		
		//rollpitchyaw(m->rpy, m->q);

		normalize(m->nq,m->q);
		q2Euler(m->rpy,m->nq);
	//scaling for degrees output
		for (int i=0;i<DIM;i++){
			m->rpy[i]*=90.f/PI_2;
			m->gyro[i]=m->g[i]/m->gsens;
			m->accel[i]=m->a[i]/m->asens;
		}

	//unwrap yaw when it reaches 180
		m->rpy[2] = wrap_180(m->rpy[2]);

	//change sign of Pitch, MPU is attached upside down
		m->rpy[1]*=-1.0;

		m->timestamp=timestamp;
	}
	return more;
}
