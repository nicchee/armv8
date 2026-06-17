#ifndef FILEIO_H
#define FILEIO_H

#include <stdint.h>
#include <stdio.h>

#define MAX_LINE_LEN 128
typedef char line_t[MAX_LINE_LEN];

extern void bin_file_write(FILE *out, uint32_t *bin);

#endif 
