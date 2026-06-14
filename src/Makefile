CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean assembler_dir

all: assembler_dir emulate

assemble: assemble.o tokeniser.o dpi_assembler.o asm_branch.o asm_sdt.o symtable.o bin_writer.o two_pass.o ins_parser.o op_funcs.o

emulate: emulate.o single_data_transfer.o dpi.o branch.o machine_state.o

emulate.o: emulate.c machine_state.c machine_state.h single_data_transfer.h dpi.h branch.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h machine_state.h 
dpi.o: dpi.c dpi.h machine_state.h  
branch.o: branch.c machine_state.h branch.h
machine_state.o: machine_state.c machine_state.h

assembler_dir:
	$(MAKE) -C assembler
clean:
	$(RM) *.o emulate
	$(MAKE) -C assembler clean
	
