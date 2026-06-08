#include "symtable.h"
#include "instruction.h"
#include "ins_parser.h" //includes tokeniser.h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool parse(char *line, uint32_t *addr, instruction_t ins) {
	TokenisedLine tl = tokenise(line);
	if (tl.opcode == NULL) {
		return false;
	}
	
	switch (get_op_t(tl)) {
	case HALT:
	
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

