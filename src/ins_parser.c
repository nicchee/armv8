#include "symtable.h"
#include "instruction.h"
#include "ins_parser.h" //includes tokeniser.h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

bool parse(char *line, uint32_t *addr, instruction_t ins) {
	TokenisedLine tl = tokenise(line);
	if (tl.opcode == NULL) {
		return false;
	}
	
	switch (get_op_t(tl)) {
	case HALT:
		ins->halt.halt_val = HALT;
		ins->op0 = HALT_OP0;
		return true;

	case INTDIR:
		
	case ADD:
		break;
	case ADDS:
		break;
	case SUB:
		break;
	case SUBS:
		break;
	case AND:
	case ANDS:
	case BIC:
	case BICS:
	case EOR:
	case EON:
	case ORR:
	case ORN:
	case MOVN:
	case MOVK:
	case MOVZ:
	case MADD:
	case MSUB:
	case B:
	case BCOND:
	case LDR:
	case STR:
	case ALIAS: // 9 Aliases in total (neg, negs counted as 2)
	// cmp <-> subs
	if (strcmp(tl.opcode, "cmp") == 0) {

	}	
	// cmn <-> adds
	else if (strcmp(tl.opcode, "cmn") == 0) {

	}	
	// neg, negs <-> sub, subs
	else if (strcmp(tl.opcode, "neg") == 0 ||
			strcmp(tl.opcode, "negs" == 0)) {

	}	
	// tst <-> ands
	else if (strcmp(tl.opcode, "tst") == 0) {

	}	
	// mvn <-> orn
	else if (strcmp(tl.opcode, "mvn") == 0) {

	}	
	// mov <-> orr
	else if (strcmp(tl.opcode, "mov") == 0) {

	}	
	// mul <-> madd
	else if (strcmp(tl.opcode, "mul") == 0) {

	}	
	// mneg <-> msub
	else if (strcmp(tl.opcode, "mneg") == 0) {

	}
	else {
		printf(stderr, "Alias not recognised");
		return false;
	}

} //end of switch statement
} //end of parse function

