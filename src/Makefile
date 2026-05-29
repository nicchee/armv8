CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o single_data_transfer.o

emulate.o: emulate.c emulate.h single_data_transfer.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h emulate.h 

clean:
	$(RM) *.o assemble emulate
	
