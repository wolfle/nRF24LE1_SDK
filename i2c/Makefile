all: test	
CFLAGS=-ggdb -Wall


test: test.c ssd1306.c bmp180.c am2320.c mpu/libmpu.a 
	gcc ${CFLAGS} -o test  am2320.c bmp180.c ssd1306.c test.c mpu/libmpu.a -lm

mpu/libmpu.a:
	$(MAKE) -C mpu

clean:
	rm -f test
	cd mpu && make clean 
