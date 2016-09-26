CC=gcc
AR=ar
CCFLAGS=-Wall -g -O2
CC_OPTS=-c -DMPU6050 -DMPU_DEBUGOFF -I./

SOURCES=sensor.c inv_mpu_lib/inv_mpu.c inv_mpu_lib/inv_mpu_dmp_motion_driver.c iica/iica.c qmath.c 
OBJECTS=$(SOURCES:.c=.o) $(SOURCES:.cc=.o)

LIB=libmpu.a

%.o: %.c
	$(CC) $(CCFLAGS) $(CC_OPTS) $< -o $@ 


$(LIB): $(OBJECTS) 
	$(AR) rcs $(LIB) $(OBJECTS) 

all: $(LIB)

clean:
	rm -rf *.o
	rm -rf inv_mpu_lib/*.o
	rm -rf iica/*.o
	rm -rf $(LIB)
