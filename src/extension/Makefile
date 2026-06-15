CC      ?= gcc
CFLAGS  ?= -std=c17 -g \
    -D_POSIX_SOURCE -D_DEFAULT_SOURCE \
    -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: translate

translate: translate.o codegen.o morsegen.o morse_table.o
	$(CC) $(CFLAGS) $^ -o $@

translate.o: translate.c morsegen.c morsegen.h codegen.c codegen.h
codegen.o: codegen.c codegen.h morse_table.c morse_table.h
morsegen.o: morsegen.c morse_table.c morse_table.h
morse_table.o: morse_table.c morse_table.h

clean:
	$(RM) *.o translate