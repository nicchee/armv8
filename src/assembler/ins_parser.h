#ifndef INS_PARSER_H
#define INS_PARSER_H

#include <stdint.h>

#define HALT_OPERAND "x0"

typedef enum {
	ADD, ADDS, 
	SUB, SUBS, 
	AND, ANDS, BIC, BICS, EOR, EON, ORR, ORN, 
	MOVN, MOVK, MOVZ, MADD, MSUB, 
	B, BR, BCOND, 
	LOAD_STORE,
	HALT, 
	INTDIR,
	ALIAS
} op_t;


extern void parse(char *line, uint32_t *addr, instruction_t ins, symtable table);

#endif
