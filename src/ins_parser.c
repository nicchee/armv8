#include "symtable.h"
#include "instruction.h"
#include "ins_parser.h" //includes tokeniser.h
#include "tokeniser.h"
#include "op_funcs.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//op_t op = get_op_t (tl);
//	returns op from tokenisedline
static op_t get_op_t (TokenisedLine tl){
	char *opcstr = tl.opcode;
	//Arithmetic
	if (strcmp(opcstr, "add") == 0) return ADD;
	else if (strcmp(opcstr, "adds") == 0) return ADDS;
	else if (strcmp(opcstr, "sub") == 0) return SUB;
	else if (strcmp(opcstr, "subs") == 0) return SUBS;
	// Logical
	else if (strcmp(opcstr, "and") == 0) {
		if(tl.num_operands == 3) {
			if ( strcmp(tl.operands[0], HALT_OPERAND) == 0 && 
			strcmp(tl.operands[1], HALT_OPERAND) == 0 &&
			strcmp (tl.operands[2], HALT_OPERAND) == 0 ) {
				return HALT;
			}
		}
		return AND;
	}
	else if (strcmp(opcstr, "ands") == 0) return ANDS;
	else if (strcmp(opcstr, "bic") == 0) return BIC;
	else if (strcmp(opcstr, "bics") == 0) return BICS;
	else if (strcmp(opcstr, "eor") == 0) return EOR;
	else if (strcmp(opcstr, "eon") == 0) return EON;
	else if (strcmp(opcstr, "orr") == 0) return ORR;
	else if (strcmp(opcstr, "orn") == 0) return ORN;

	// Wide move
	else if (strcmp(opcstr, "movn") == 0) return MOVN;
	else if (strcmp(opcstr, "movk") == 0) return MOVK;
	else if (strcmp(opcstr, "movz") == 0) return MOVZ;

	// Multiply
	else if (strcmp(opcstr, "madd") == 0) return MADD;
	else if (strcmp(opcstr, "msub") == 0) return MSUB;

	// Branch
	else if ((strcmp(opcstr, "b") == 0 || 
		strcmp(opcstr, "br") == 0) ) return B;
	else if (strcmp(opcstr, "b.") == 0) return BCOND;

	// Load/store
	else if (strcmp(opcstr, "ldr") == 0 || 
		strcmp(opcstr, "str") == 0) return LOAD_STORE;

	// .int directive 
	else if (strcmp(opcstr, ".int") == 0) return INTDIR;
	
	// Any Aliases
	// Replace the 'else' with explicit alias checks
	else if (
		strcmp(opcstr, "cmp") == 0 || strcmp(opcstr, "cmn") == 0 ||
        strcmp(opcstr, "neg") == 0 || strcmp(opcstr, "negs") == 0 ||
        strcmp(opcstr, "tst") == 0 || strcmp(opcstr, "mvn") == 0 ||
        strcmp(opcstr, "mov") == 0 || strcmp(opcstr, "mul") == 0 ||
        strcmp(opcstr, "mneg") == 0
	) return ALIAS;
	
	// If it's not a known instruction AND not a known alias, it's a typo
	else {
    	fprintf(stderr, "Syntax Error: Unknown instruction '%s'\n", opcstr);
    	exit(EXIT_FAILURE);
	}
}

// Helper function to parse all aliases
static void parse_alias(symtable table, TokenisedLine *tl, uint32_t *addr, instruction_t ins) {
	//cmp rn, <op2> -> subs rzr, rn, <op2>
	if (strcmp(tl->opcode, "cmp") == 0) {
		shift_all_ops(&tl);
		set_op_to_rzr(&tl, 0, 1);
		(*op_func_arr[SUBS]) (table, *tl, addr, ins);
	}	
	// cmn rn, <op2> -> adds rzr, rn, <op2>
	else if (strcmp(tl->opcode, "cmn") == 0) {
		shift_all_ops(&tl);
		set_op_to_rzr(&tl, 0, 1);
		(*op_func_arr[ADDS]) (table, *tl, addr, ins);
	}	
	// neg(s) rd, <op2> <-> sub(s) rd, rzr, <op2>
	else if (strcmp(tl->opcode, "neg") == 0) {
		tl.operands[2] = tl.operands[1];
		set_op_to_rzr(&tl, 1, 0);
		(*op_func_arr[SUB]) (table, *tl, addr, ins);
	}
	else if	(strcmp(tl->opcode, "negs") == 0) {
		tl.operands[2] = tl.operands[1];
		set_op_to_rzr(&tl, 1, 0);
		(*op_func_arr[SUBS]) (table, *tl, addr, ins);
	}	
	// tst rn, <op2> -> ands rzr, rn, <op2>
	else if (strcmp(tl->opcode, "tst") == 0) {
		shift_all_ops(&tl);
		set_op_to_rzr(&tl, 0, 1);
		(*op_func_arr[ANDS]) (table, *tl, addr, ins);
	}	
	// mvn rd, <op2> -> orn rd, rzr, <op2>
	else if (strcmp(tl->opcode, "mvn") == 0) {
		tl.operands[2] = tl.operands[1];
		set_op_to_rzr(&tl, 1, 0);
		(*op_func_arr[ORN]) (table, *tl, addr, ins);
	}	
	// mov rd, rm -> orr rd, rzr, rm
	else if (strcmp(tl->opcode, "mov") == 0) {
		tl.operands[2] = tl.operands[1];
		set_op_to_rzr(&tl, 1, 0);
		(*op_func_arr[ORR]) (table, *tl, addr, ins);
	}	
	// mul rd, rn, rm -> madd rd, rn, rm, rzr
	else if (strcmp(tl->opcode, "mul") == 0) {
		set_op_to_rzr(&tl, 3, 0);
		(*op_func_arr[MADD]) (table, *tl, addr, ins);
	}	
	// mneg rd, rn, rm -> msub rd, rn, rm, rzr 
	// Get_op_t already checks that Alias is valid so must be mneg
	else { 
		set_op_to_rzr(&tl, 3, 0);			
		(*op_func_arr[MSUB]) (table, *tl, addr, ins);
	}
} 



//parse(rawline, curraddr, ins)
//	parse raw line to insert value into parts of the ins struct
void parse(char *line, uint32_t *addr, instruction_t ins, symtable table) {
	// STUB
	// check line is null terminated (IMPORT THIS FROM TWO PASS NULL TERMINATED HELPER FUNCTION!)
	
	TokenisedLine tl = tokenise(line);
	
	// checks if opcode is missing
	if (tl.opcode == NULL) {
		fprintf(stderr, "Opcode is missing!\n");
		exit(EXIT_FAILURE);
	}
	
	op_t op = get_op_t(tl);

	if (op == HALT) {
		ins->halt.halt_val = HALT_INS;
		ins->op0 = HALT_OP0;
	}
	else if (op == INTDIR) {
		

	}
	else if (op == ALIAS) {
		parse_alias(table, &tl, addr, ins);
	}	
	// Non-alias functions: use function pointer array from op_funcs.c
	else {
		if (op_func_arr[op] != NULL) {
			(*op_func_arr[op]) (table, tl, addr, ins);
		} else fprintf(stderr, "Function pointer in op_func_arr was null");
	}


}





