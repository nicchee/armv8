CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o tokeniser.o dpi_assembler.o asm_branch.o asm_sdt.o

emulate: emulate.o single_data_transfer.o dpi.o branch.o machine_state.o

emulate.o: emulate.c machine_state.c machine_state.h single_data_transfer.h dpi.h branch.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h machine_state.h 
dpi.o: dpi.c dpi.h machine_state.h  
branch.o: branch.c machine_state.h branch.h
machine_state.o: machine_state.c machine_state.h

tokeniser.o: tokeniser.c tokeniser.h
dpi_assembler.o: dpi_assembler.c dpi_assembler.h
asm_branch.o: asm_branch.c asm_branch.h
asm_sdt.o: asm_sdt.c asm_sdt.h

clean:
	$(RM) *.o assemble emulate
	
