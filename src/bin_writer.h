#ifndef FILEIO_H
#define FILEIO_H

#include "symtable.h"

#include <stdint.h>

#define MAX_LINE_LEN 128
typedef char line_t[MAX_LINE_LEN];

extern void bin_file_write(const char *fileName, uint32_t *bin);

#endif 
