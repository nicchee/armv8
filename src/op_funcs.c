#include "op_funcs.h"
#include "ins_parser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//if (operand_starts_with(tl, index, c)) {..}
//	return true if operand at index start with c
static int operand_starts_with(TokenisedLine tl, int index, char c) {
	return *(tl.operands[index]) == c;
}

	// ADDS, 
	// SUB, SUBS, 
	// AND, ANDS, BIC, BICS, EOR, EON, ORR, ORN, 
	// MOVN, MOVK, MOVZ, MADD, MSUB, 
	// B, BCOND, 
	// LDR, STR, 
	// HALT, 
	// INTDIR,
	// ALIAS

static uint8_t reg_index(char *reg) {
	return atoi(reg + 1);
} 

static int32_t s_ext(char *imm, int bits) {
	int imm_val = atoi(imm + 1);
	int shift = 32 - bits;

	//shift value to MSB
	//typecase
	//logical right shift
	return ((int32_t)(imm_val << shift)) >> shift; 
}

static uint32_t z_ext(char *imm) {
	int imm_val = atoi(imm + 1); //remove '#'

	return 0xffff & imm_val;
}

// op functions:
static void parseADD(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	if (operand_starts_with(tl, 2, '#')) { // dpi imm
		ins->op0 = DPI_OP0;
		ins->dp_imm.sf = operand_starts_with(tl, 0, 'x');
		ins->dp_imm.opc = ADD_OPC;
		ins->dp_imm.opi = ARITH_OPI;
		ins->dp_imm.rd = reg_index(tl.operands[0]);
		ins->dp_imm.arith.rn = reg_index(tl.operands[1]);
		ins->dp_imm.arith.sh = tl.num_operands == 5 && (strcmp(tl.operands[4], "#12") == 0);  // sh == 1 (lsl imm by 12)
		ins->dp_imm.arith.imm12 = z_ext(tl.operands[2]);
	} else { // dpi reg 
		ins->op0 = DPR_OP0;
		ins->dp_reg.rd = reg_index(tl.operands[0]);
		ins->dp_reg.rn = reg_index(tl.operands[1]);
		ins->dp_reg.rm = reg_index(tl.operands[2]);
		ins->dp_reg.sf = operand_starts_with(tl, 0, 'x');
		ins->dp_reg.opc = ADD_OPC;
		ins->dp_reg.M = 0; // arith
		ins->dp_reg.opr = ARITH_OPR;
		if (tl.num_operands == 5) { //shift 
			if (strcmp(tl.operands[3], "lsl")) {
				ins->dp_reg.arith_logic.shift = LSL_SHIFT;
			} else if (strcmp(tl.operands[3], "lsr")) {
				ins->dp_reg.arith_logic.shift = LSR_SHIFT;
			} else { // asr
				ins->dp_reg.arith_logic.shift = ASR_SHIFT;
			}
			ins->dp_reg.arith_logic.imm6 = z_ext(tl.operands[4]);
		} else {
			// no shift 
			ins->dp_reg.arith_logic.shift = LSL_SHIFT;
			ins->dp_reg.arith_logic.imm6 = 0;
		}
		ins->dp_reg.arith_logic.N = 0; // arith should not have N
	}
}

static void parseADDS(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	
}


// array of function pointers
op_func_ptr op_func_arr[] = {
    [ADD] = &parseADD,

};

