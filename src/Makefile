CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate translator

assemble: assemble.o tokeniser.o dpi_assembler.o asm_branch.o asm_sdt.o symtable.o bin_writer.o two_pass.o ins_parser.o op_funcs.o

emulate: emulate.o single_data_transfer.o dpi.o branch.o machine_state.o

translator: translator.o part4/codegen.o part4/morsegen.o part4/morse_table.o

assemble.o: assemble.c instruction.h
emulate.o: emulate.c machine_state.c machine_state.h single_data_transfer.h dpi.h branch.h
single_data_transfer.o: single_data_transfer.c single_data_transfer.h machine_state.h 
dpi.o: dpi.c dpi.h machine_state.h  
branch.o: branch.c machine_state.h branch.h
machine_state.o: machine_state.c machine_state.h

tokeniser.o: tokeniser.c tokeniser.h
dpi_assembler.o: dpi_assembler.c dpi_assembler.h instruction.h
asm_branch.o: asm_branch.c asm_branch.h instruction.h
asm_sdt.o: asm_sdt.c asm_sdt.h instruction.h
symtable.o: symtable.c symtable.h
bin_writer.o: bin_writer.c bin_writer.h symtable.h
two_pass.o: two_pass.c two_pass.h symtable.h
ins_parser: ins_parser.c ins_parser.h 
op_funcs.o: op_funcs.c op_funcs.h

#translator
translator.o: translator.c part4/morsegen.c part4/morsegen.h part4/codegen.c part4/codegen.h
part4/codegen.o: part4/codegen.c part4/codegen.h part4/morse_table.c part4/morse_table.h
part4/morsegen.o: part4/morsegen.c part4/morse_table.c part4/morse_table.h
part4/morse_table.o: part4/morse_table.c part4/morse_table.h

clean:
	$(RM) *.o part4/*.o assemble emulate translator
	
