#ifndef BMP180_H_
#define BMP180_H_

#define BMP180_OSS_SINGLE 0
#define BMP180_OSS_DOUBLE 1
#define BMP180_OSS_QUALDRAL 2
#define BMP180_OSS_OCTAL 3


PT_V(bmp180_init);
PT(bmp180_measure_t,int32_t *t);
PT(bmp180_measure_p,uint8_t oss,int32_t *p);
//uint16_t bmp180_altitude(uint32_t p);

#endif
