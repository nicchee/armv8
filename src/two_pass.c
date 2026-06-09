#include <stdlib.h>
#include <stdio.h> 

#include "two_pass.h"
#include "symtable.h"
#include "fileio.h"

void first_pass(FILE *in) {
    line_t line;
    while ( fgets(line, sizeof(line), in) != NULL) {
		if (*line == '\n') { continue; } //empty line
		int len = strlen(line);
        line[--len] = '\0'; //remove '\n' from line
		if (len > 0 && !feof(in)) {
			fprintf(stderr, "Line longer than max length of %d, not passed to parser\n", MAX_LINE_LEN-1);
			fclose(in);
			exit(EXIT_FAILURE);
		}
		parse_ins(line);
	}
}
