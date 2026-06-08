#include "fileio.h"

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
	size_t bytes_written = fwrite(bin, sizeof(uint32_t), 1, in);
	if (bytes_written != 1) {
		fprintf(stderr, "Error writing binary to file\n");
		exit(1);
	}
	fclose(in);
}

// Assembly file reader - passes each line to instruction parser
void asm_file_read(const char* fileName) {
	FILE *in = fopen(fileName, "r");
	if (in == NULL) {
		fprintf(stderr, "Can't open assembly file\n");
		exit(1);
	}

	line_t line;
	// if line len > MAX_LINE_LEN -1, likely to be syntactically wrong, ignore this line and don't send to parser
	while ( fgets(line, sizeof(line), in) != NULL) {
		if (*line == '\n') { continue; } //empty line
		int len = strlen(line);
		if (len > 0 && line[len - 1] != '\n' && !feof(in)) {
			fprintf(stderr, "Line longer than max length of %d, not passed to parser\n", MAX_LINE_LEN-1);
			fclose(in);
			exit(EXIT_FAILURE);
		}
		parse_ins(line);
	}
	fclose(in);
}
	
