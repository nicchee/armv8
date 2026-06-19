#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bin_writer.h"

//uint32_t result;
//bin_file_write(out, &result)
//	Writes a binary num to given bin file
void bin_file_write(FILE *out, uint32_t *bin) {
	size_t bytes_written = fwrite(bin, sizeof(uint32_t), 1, out);
	if (bytes_written != 1) {
		fprintf(stderr, "Error writing binary to file\n");
		exit(1);
	}
}
