#include <math.h>

// Quaternion multiplication is defined by:
//     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
//     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
//     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
//     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2

//NOTE: result MUST differ from q1 and q2
void qproduct(float * result, float * q1, float *q2){ // q1 * q2 => result 
	result[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
	result[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
	result[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
	result[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
}

void qconjugate(float * c, float *q){
	c[0]=q[0];
	c[1]=-q[1];
	c[2]=-q[2];
	c[3]=-q[3];
}
        
float qabs(long * q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
}
        
void normalize(float *nq, long *q) {
    float m = qabs(q);
	nq[0] = q[0]/m;
	nq[1] = q[1]/m;
	nq[2] = q[2]/m;
	nq[3] = q[3]/m;
}

//rotate the quaternion v according to q
void qrotate(float * result, float * v, float *q){
	float c[4], temp[4];
	qconjugate(c,q);
	qproduct(temp,q,v);
	qproduct(result,temp,c);
}
//rotate back the rotated quaternion v to its unrotated
void qderotate(float * result, float * v, float *q){
	float c[4], temp[4];
	qconjugate(c,q);
	qproduct(temp,c,v);
	qproduct(result,temp,q);
}

//translate vector from body frame to inert frame
void qbody2inert(float *i, float*b, float*q){ // i and b is quaternion
	qrotate(i,b,q);
}

//translate vector from inert frame to body frame
void qinert2body(float *b, float*i, float*q){ // i and b is quaternion
	qderotate(b,i,q);
}

#define EPSILON         0.0001f
#define PI_2            1.57079632679489661923f

void q2Euler( float * rpy, float* nq ){	
	float sqy = nq[2] * nq[2];
	float sqz = nq[3] * nq[3];
	float sqw = nq[0] * nq[0];

	float test = nq[1] * nq[3] - nq[0] * nq[2];

	if( test > 0.5f - EPSILON ){
			rpy[0] = 2.f * atan2( nq[2], nq[0] );
			rpy[1] = PI_2;
			rpy[2] = 0;
	}
	else if( test < -0.5f + EPSILON )
	{
			rpy[0] = -2.f * atan2( nq[2], nq[0] );
			rpy[1] = -PI_2;
			rpy[2] = 0;
	}
	else
	{
			rpy[0] = atan2( 2.f * ( nq[1] * nq[0] + nq[2] * nq[3] ), 1.f - 2.f * ( sqz + sqw ) );
			rpy[1] = asin( 2.f * test );
			rpy[2] = atan2( 2.f * ( nq[1] * nq[2] - nq[3] * nq[0] ), 1.f - 2.f * ( sqy + sqz ) );
	}
}
