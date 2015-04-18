CFLAGS=-Iinclude --model-large --opt-code-size
DFLAGS=-Iinclude -MM
SDCC=sdcc
SDCCLIB=sdcclib
SED=sed

all: Makefile.gen libs

clean:
	-rm -f _target*/obj/* _target*/dep/* _target*/lib/* Makefile.gen

Makefile.gen: configure.py
	./configure.py

.PHONY: all clean libs

-include Makefile.gen
