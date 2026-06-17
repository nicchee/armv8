#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "two_pass.h"
#include "instruction.h"
#include "tokeniser.h"
#include "bin_writer.h"
#include "ins_parser.h"
#include "asm_sdt.h"
#include "asm_branch.h"
#include "dpi_assembler.h" 

//uint32_t result = assemble_bin(ins); 
//	Helper function for second_pass() 
//	Binary encoding using functions in asm-branch, asm-sdt, dpi_assembler
static uint32_t assemble_bin(instruction_t ins) {
	switch (ins->op0) {
	    	case DPI_OP0:
			return dpi_imm_assembler(ins);
	    	case DPR_OP0:
			return dpi_reg_assembler(ins);
		case DPR_MUL_OP0:
			return dpi_reg_assembler(ins);
		case BRANCH_OP0:
			return assemble_branch(ins);
		case BRANCH_REG_OP0:
			return assemble_branch(ins);
		case LOAD_STORE_OP0:
			return assemble_load_store(ins);
		case HALT_OP0:
			return HALT_INS;
		case INTDIR_OP0:
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
		if (strlen(line) > (MAX_LINE_LEN-1) && !feof(in)) {
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
			if (!insert_symkv(table, tl.label, currAddr)) {
				fprintf(stderr, "label not null but failed to insert into table");
				exit(EXIT_FAILURE);
			}
		}
		free_tl(&tl);	
	}
}

//symtable table;
//second_pass(in, table, out_name);
//	second pass generates uint32_t binary number and writes to out file
void second_pass(FILE *in, symtable table, const char *out_name) {
    line_t line;
    uint32_t currAddr = 0;
    rewind(in);

    FILE *out = fopen(out_name, "wb");
    if (out == NULL) {
	fprintf(stderr, "Error opening bin file");
	exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), in) != NULL) {
	if (*line == '\n') { continue; } //empty line
	line[strcspn(line,"\n")] = '\0'; //remove '\n' from line and null terminate
	
	//Replace any labels with address from symtable 
	instruction_t ins = malloc(sizeof(struct instruction_t));
	parse(line, &currAddr, ins, table);
	if (ins->op0 == EMPTY_SKIP_OP0) continue;

	if (ins->op0 == INTDIR_OP0 && ins->int_dir.is_label) {
		ins->int_dir.int_val = get_addr(table, ins->int_dir.label);
		free(ins->int_dir.label);
		ins->int_dir.label = NULL;
	}

	//Generate binary encoding and write to file
	uint32_t result = assemble_bin(ins);
	bin_file_write(out, &result);	
	currAddr += 4;

	free(ins);
	ins = NULL;
	}
}
