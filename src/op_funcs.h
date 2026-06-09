#ifndef OP_FUNCS_H
#define OP_FUNCS_H

#include <stdint.h>
#include "tokeniser.h"
#include "instruction.h"

typedef void (*op_func_ptr) (TokenisedLine tl, uint32_t *addr, instruction_t ins); 

extern op_func_ptr op_func_arr[]; 

#endif