CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o single_data_transfer.o dpi.o branch.o

emulate.o: emulate.c emulate.h single_data_transfer.h dpi.h branch.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h emulate.h 
dpi.o: dpi.c dpi.h emulate.h  
branch.o: branch.c emulate.h branch.h

clean:
	$(RM) *.o assemble emulate
	
