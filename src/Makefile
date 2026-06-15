CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean emulator_dir assembler_dir extension_dir

all: assembler_dir emulator_dir extension_dir

emulator_dir:
	$(MAKE) -C emulator
	cp emulator/emulate ./emulate

assembler_dir:
	$(MAKE) -C assembler
	cp assembler/assemble ./assemble

extension_dir: 
	$(MAKE) -C extension
	cp extension/translate ./translate

clean:
	$(MAKE) -C emulator clean
	$(MAKE) -C assembler clean
	$(MAKE) -C extension clean
	$(RM) *.o assemble emulate translate

