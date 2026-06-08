#ifndef INS_PARSER_H
#define INS_PARSER_H

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
extern op_t get_op_t (TokenisedLine tl);

#endif