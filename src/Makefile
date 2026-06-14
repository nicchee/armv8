# CC      ?= gcc
# CFLAGS  ?= -std=c17 -g\
# 	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
# 	-Wall -Werror -pedantic

# .SUFFIXES: .c .o

# .PHONY: all clean

# all: assemble emulate

# assemble: assemble.o

# emulate: emulate.o single_data_transfer.o dpi.o branch.o machine_state.o

# emulate.o: emulate.c machine_state.c machine_state.h single_data_transfer.h dpi.h branch.h
# single_data_transfer.o: single_data_transfer.c single_data_transfer.h machine_state.h 
# dpi.o: dpi.c dpi.h machine_state.h  
# branch.o: branch.c machine_state.h branch.h
# machine_state.o: machine_state.c machine_state.h

# clean:
# 	$(RM) *.o assemble emulate
	
CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean emulator_dir assembler_dir

all: assembler_dir emulator_dir

emulator_dir:
	$(MAKE) -C emulator
	cp emulator/emulate ./emulate

assembler_dir:
	$(MAKE) -C assembler
	cp assembler/assemble ./assemble

clean:
	$(MAKE) -C emulator clean
	$(MAKE) -C assembler clean
	$(RM) *.o part4/*.o assemble emulate translator

