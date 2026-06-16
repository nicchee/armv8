CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate

emulate: emulate.o single_data_transfer.o dpi.o branch.o machine_state.o instruction.o
	$(CC) $(CFLAGS) $^ -o $@

emulate.o: emulate.c machine_state.c machine_state.h single_data_transfer.h dpi.h branch.h instruction.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h machine_state.h instruction.h
dpi.o: dpi.c dpi.h machine_state.h instruction.h
branch.o: branch.c machine_state.h branch.h instruction.h
machine_state.o: machine_state.c machine_state.h
instruction.o: instruction.c instruction.h machine_state.h

clean:
	$(RM) *.o emulate
