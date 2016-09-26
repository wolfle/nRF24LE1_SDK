#ifndef _QMATH_H_
#define _QMATH_H_
//long quaternion data >> 14 turn to float
#define PI_2            1.57079632679489661923f

void qproduct(float * result, float * q1, float *q2);
void qconjugate(float * c, float *q);
float qabs(long * q);
void normalize(float *nq,long *q);
void qrotate(float * result, float * v, float *q);
void qderotate(float * result, float * v, float *q);
void qbody2inert(float *i, float *b, float*q);
void qinert2body(float *b, float*i, float*q);
void q2Euler( float * rpy, float * nq );
#endif
