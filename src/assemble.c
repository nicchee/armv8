#include "symtable.h"
#include "fileio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  	if (argc != 3) {
		fprintf(stderr, "Usage: %s <assembly_file_in> <binary_file_out>", argv[0]);
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

