CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o branch.o

emulate.o: emulate.c emulate.h branch.h
branch.o: branch.c branch.h emulate.h

clean:
	$(RM) *.o assemble emulate
	
