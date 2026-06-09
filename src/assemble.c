#include "symtable.h"
#include "instruction.h"
//#include "fileio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  	if (argc != 3) {
		fprintf(stderr, "Usage: %s <assembly_file_in> <binary_file_out>", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	FILE *in = fopen(fileName, "r");
	if (in == NULL) {
		fprintf(stderr, "Can't open assembly file");
		exit(1);
	}

	char line[MAX_LINE_LEN];
	int len = strlen(line);
	// if line len > MAX_LINE_LEN -1, likely to be syntactically wrong, ignore this line and don't send to parser
	while (fgets(line, sizeof(line), in) != NULL) {
		if (len > 0 && line[len-1] != '\n' && !feof(in)) {
			fprintf(stderr, "Line longer than max length of %d, not passed to parser", MAX_LINE_LEN-1);
			int ch;
			while ((ch = getc(in)) != EOF && ch != '\n') {
				//empty body - ignore rest of line
			}
		continue;
		}
	parse_ins(line);
	} 
	fclose(in);

	return EXIT_SUCCESS;
}

