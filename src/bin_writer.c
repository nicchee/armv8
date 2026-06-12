#include "bin_writer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Writes a binary num to given bin file
void bin_file_write(const char *fileName, uint32_t *bin) {
	FILE *in = fopen(fileName, "wb");
	if (in == NULL)  {
		fprintf(stderr, "Error opening bin file\n");
		exit(EXIT_FAILURE);
	}
	size_t bytes_written = fwrite(&bin, sizeof(uint32_t), 1, in);
	if (bytes_written != 1) {
		fprintf(stderr, "Error writing binary to file\n");
		exit(1);
	}
	fclose(in);
}
