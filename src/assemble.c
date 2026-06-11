#include "symtable.h"
#include "instruction.h"
#include "two_pass.h"
#include "bin_writer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  	if (argc != 3) {
		fprintf(stderr, "Usage: %s <assembly_file_in> <binary_file_out>", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		fprintf(stderr, "Error opening assembly file");
		exit(EXIT_FAILURE);
	}

	symtable table = init_table();
	first_pass(in, table);
	second_pass(in, table, argv[2]);

	free_table(table);

	return EXIT_SUCCESS;
}

