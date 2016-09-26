CC=gcc
AR=ar
CCFLAGS=-Wall -g -O2
CC_OPTS=-c -DMPU6050 -DMPU_DEBUGOFF -I./

PROG=mdtest

LIBD=mpu

SOURCES=main.c
LIB=$(LIBD)/libmpu.a -lm

all: $(PROG)

$(PROG): $(SOURCES) $(LIB)
	$(CC) $(LDFLAG) $(CCFLAGS) -o $(PROG) $(SOURCES) $(LIB)
	
%.o: %.c
	$(CC) $(CCFLAGS) $(CC_OPTS) $< -o $@ 


$(LIB):
	$(MAKE) -C $(LIBD)/

clean:
	rm -rf *.o *~ *.mod
	rm -rf $(PROG)
	cd $(LIBD) && $(MAKE) clean
