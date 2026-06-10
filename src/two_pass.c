#include <stdlib.h>
#include <stdio.h> 

#include "two_pass.h"
#include "symtable.h"
#include "fileio.h"

//first_pass (in);
// 	pass through the whole assemble (.s) file and insert LABEL and its address into symtable
//	start with curraddr = 0. if its not a label, curraddr += 4; else insert_symkv(..)
void first_pass(FILE *in) {
    line_t line;
    while ( fgets(line, sizeof(line), in) != NULL) {
		if (*line == '\n') { continue; } //empty line
		int len = strlen(line);
        line[--len] = '\0'; //remove '\n' from line and null terminate
		if (len > 0 && !feof(in)) {
			fprintf(stderr, "Line longer than max length of %d, not passed to parser\n", MAX_LINE_LEN-1);
			fclose(in);
			exit(EXIT_FAILURE);
		}
		parse(line);
	}
}

// NOT SURE THE FORM IT TAKES YET
// IDEA IS TO TAKE EVERY LINE OF THE .S FILE, CALL PARSE ON IT.
// void second_pass(FILE *in) {

// }
