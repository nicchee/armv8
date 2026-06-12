#ifndef INS_PARSER_H
#define INS_PARSER_H

#include <stdint.h>

#define HALT_OPERAND "x0"

// Using an enum type op_t so switch case can be used in parser
typedef enum {
	ADD, ADDS, 
	SUB, SUBS, 
	AND, ANDS, BIC, BICS, EOR, EON, ORR, ORN, 
	MOVN, MOVK, MOVZ, MADD, MSUB, 
	B, BCOND, 
	LOAD_STORE,
	HALT, 
	INTDIR,
	ALIAS
} op_t;


extern void parse(char *line, uint32_t *addr, instruction_t ins, symtable table);

#endif
