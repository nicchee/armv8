#include "symtable.h"

extern void bin_file_write(const char* fileName, uint32_t *bin);

int main(int argc, char **argv) {
  	if (argc != 3) {
		fprintf(stderr, "Usage: %s <assembly_file_in> <binary_file_out>", argv[0]);
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

// Writes a binary num to given bin file
void bin_file_write(const char *fileName, uint32_t *bin) {
	FILE *in = fopen(fileName, "wb");
	if (in == NULL)  {
		fprintf(stderr, "Error opening bin file");
		exit(EXIT_FAILURE);
	}
	size_t bytes_written = fwrite(bin, sizeof(uint32_t), 1, in);
	if (bytes_written != 1) {
		fprintf(stderr, "Error writing binary to file");
		exit(1);
	}
	fclose(in);
}
