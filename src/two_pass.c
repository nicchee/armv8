#include <stdlib.h>
#include <stdio.h> 

#include "two_pass.h"
#include "symtable.h"
#include "fileio.h"
#include "ins_parser.h"
#include "asm-sdt.h"
#include "asm-branch.h"
#include "dpi_assembler.h"

//Helper function for second_pass() 
//Binary encoding using functions in asm-branch, asm-sdt, dpi_assembler
static uint32_t assemble_bin(instruction_t ins) {
	switch (ins->op0) {
	    	case DPI_OP0:
			return dp_imm_assembler(ins);
	    	case DPR_OP0:
			return dp_reg_assembler(ins);
		case BRANCH_OP0:
			return assemble_load_store(ins);
		case LOAD_STORE_OP0:
			return assemble_branch(ins);
		case HALT_OP0:
			return HALT_INS;
		case INT_DIR_OP0:
			return ins->int_dir.int_val;
		default:
			fprintf(stderr, "op0 in instruction_t is unknown type");
			exit(EXIT_FAILURE);
	}
}
//first_pass (in, symtable);
// 	pass through the whole assembly (.s) file and insert LABEL and its address into symtable
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

// Second pass returns uint32_t binary number for bin file writer
uint32_t second_pass(FILE *in, symtable table) {
    line_t line;
    uint32_t currAddr = 0;
    rewind(in);

    while (fgets(line, sizeof(line), in) != NULL) {
	if (*line == '\n') { continue; } //empty line
	line[strcspn(line,"\n")] = '\0'; //remove '\n' from line and null terminate
	TokenisedLine tl = tokenise(line);
	
	//Replace any labels with address from symtable 
	instruction_t ins = malloc(sizeof(struct instruction_t));
	parse(line, currAddr, ins, table);
	if (ins->int_dir.is_label) {
		ins->int_dir.int_val = get_addr(table, label);
		free(ins->int_dir.label);
		ins->int_dir.label = NULL;
	}

	//Generate binary encoding
	uint32_t bin = assemble_bin(ins);	
		
	free(ins);
	free(&tl);
	fclose(in);
	return bin;
}

