#ifndef OP_FUNCS_H
#define OP_FUNCS_H

#include <stdint.h>
#include "symtable.h"
#include "tokeniser.h"
#include "instruction.h"

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

typedef void (*op_func_ptr) (symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins); 

extern op_func_ptr op_func_arr[]; 

#endif
