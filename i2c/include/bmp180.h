#ifndef BMP180_H_
#define BMP180_H_

#define BMP180_OSS_SINGLE 0
#define BMP180_OSS_DOUBLE 1
#define BMP180_OSS_QUALDRAL 2
#define BMP180_OSS_OCTAL 3


PT_V(bmp180_init);//3
PT_V(bmp180_measure_t); //3
PT(bmp180_measure_p, int8_t oss,__pdata int32_t * p);//3
//uint16_t bmp180_altitude(uint32_t p);

#endif
