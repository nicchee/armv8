CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble

assemble: assemble.o tokeniser.o dpi_assembler.o asm_branch.o asm_sdt.o symtable.o bin_writer.o two_pass.o ins_parser.o op_funcs.o
	$(CC) $(CFLAGS) $^ -o $@

assemble.o: assemble.c instruction.h
tokeniser.o: tokeniser.c tokeniser.h
dpi_assembler.o: dpi_assembler.c dpi_assembler.h instruction.h
asm_branch.o: asm_branch.c asm_branch.h instruction.h
asm_sdt.o: asm_sdt.c asm_sdt.h instruction.h
symtable.o: symtable.c symtable.h
bin_writer.o: bin_writer.c bin_writer.h symtable.h
two_pass.o: two_pass.c two_pass.h symtable.h
ins_parser: ins_parser.c ins_parser.h 
op_funcs.o: op_funcs.c op_funcs.h

clean:
	$(RM) *.o assemble
	
