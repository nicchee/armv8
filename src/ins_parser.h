#include "tokeniser.h"

// Using an enum type op_t so switch case can be used in parser
typedef enum {
	ADD, ADDS, 
	SUB, SUBS, 
	AND, ANDS, BIC, BICS, EOR, EON, ORR, ORN, 
	MOVN, MOVK, MOVZ, MADD, MSUB, 
	B, BCOND, 
	LDR, STR, 
	HALT, 
	INTDIR,
	ALIAS
} op_t;
// Use B for both literal and register in branching

#define HALT_OPERAND "x0"

op_t get_op_t (TokenisedLine tl){
	char *opcstr = tl.opcode;
	//Arithmetic
	if (strcmp(opcstr, "add") == 0) {
		if(tl.num_operands == 3) {
			if ( strcmp(tl.operands[0], HALT_OPERAND) == 0 && 
			strcmp(tl.operands[1], HALT_OPERAND) == 0 &&
			strcmp (tl.operands[2], HALT_OPERAND) == 0 ) {
				return HALT;
			}
		}
		return ADD;
	}
	else if (strcmp(opcstr, "adds") == 0) return ADDS;
	else if (strcmp(opcstr, "sub") == 0) return SUB;
	else if (strcmp(opcstr, "subs") == 0) return SUBS;

	// Logical
	else if (strcmp(opcstr, "and") == 0) return AND;
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
	else if (strcmp(opcstr, "b.cond") == 0) return BCOND;

	// Load/store
	else if (strcmp(opcstr, "ldr") == 0) return LDR;
	else if (strcmp(opcstr, "str") == 0) return STR;

	// .int directive 
	else if (strcmp(opcstr, ".int") == 0) return INTDIR;

	// Anything else must be Aliases	
	else return ALIAS;
}
