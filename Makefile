CFLAGS=-Iinclude 
DFLAGS=-Iinclude -MM
SDCC=sdcc
SDAR=sdar
SED=sed

all: Makefile.gen libs

clean:
	-rm -f _target*/obj/* _target*/dep/* _target*/lib/* Makefile.gen

Makefile.gen: configure.py
	./configure.py

.PHONY: all clean libs

-include Makefile.gen
