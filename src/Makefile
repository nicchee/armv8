CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean emulator_dir assembler_dir translator_dir

all: assembler_dir emulator_dir translator_dir

emulator_dir:
	$(MAKE) -C emulator
	cp emulator/emulate ./emulate

assembler_dir:
	$(MAKE) -C assembler
	cp assembler/assemble ./assemble

translator_dir: 
	$(MAKE) -C translator
	cp translator/translate ./translate

clean:
	$(MAKE) -C emulator clean
	$(MAKE) -C assembler clean
	$(MAKE) -C translator clean
	$(RM) *.o assemble emulate translate

