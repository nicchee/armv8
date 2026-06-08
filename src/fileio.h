#ifndef FILEIO_H
#define FILEIO_H

#include <stdint.h>

#endif

#define MAX_LINE_LEN 128

extern void bin_file_write(const char* fileName, uint32_t *bin);
extern void asm_file_read(const char* fileName);
extern void parse_ins(char * line);

