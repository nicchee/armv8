#include <stdlib.h>
#include <stdio.h> 

#include "two_pass.h"
#include "symtable.h"
#include "fileio.h"
#include "ins_parser.h"

//first_pass (in, symtable);
// 	pass through the whole assemble (.s) file and insert LABEL and its address into symtable
//	start with curraddr = 0. if its not a label, curraddr += 4; else insert_symkv(..)
void first_pass(FILE *in, symtable table) {
    line_t line;
    uint32_t currAddr = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
		if (*line == '\n') { continue; } //empty line
		line[strcspn(line,"\n")] = '\0'; //remove '\n' from line and null terminate
		if (len > 0 && !feof(in)) {
			fprintf(stderr, "Line longer than max length of %d, not passed to parser\n", MAX_LINE_LEN-1);
			fclose(in);
			exit(EXIT_FAILURE);
		}

		TokenisedLine tl = tokenise(line);
		if (tl.label == NULL) { //is not label -> skip
			currAddr += 4;
			free_tl(&tl);
			continue;
		} else { //is label -> insert into table
			if (!insert_symkv(table, label, currAddr)) {
				fprintf(stderr, "label not null but failed to insert into table");
				exit(EXIT_FAILURE);
			}
		}
		currAddr += 4;
		free_tl(&tl);	
	}
	fclose(in);
}

void second_pass(FILE *in, symtable table) {
    line_t line;
    uint32_t currAddr = 0;
    rewind(in);

    while (fgets(line, sizeof(line), in) != NULL) {
	if (*line == '\n') { continue; } //empty line
	line[strcspn(line,"\n")] = '\0'; //remove '\n' from line and null terminate
	TokenisedLine tl = tokenise(line);

	if (tl.opcode != NULL) {
		instruction_t ins = malloc(sizeof(struct instruction_t));
		parse(line, currAddr, ins, table);
	}

	free(&tl);
	fclose(in);

}
