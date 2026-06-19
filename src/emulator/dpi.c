#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "machine_state.h"
#include "dpi.h"
#include "instruction.h"

// Arithmetic and Logical
#define ARITH_OPR_MASK 9
#define ARITH_BL_M 0

// DPI Reg OpGroup
typedef enum {
    UNKNOWN,
    ARITHMETIC,
    BIT_LOGIC,
    MULTIPLY
} OpGroup;

// Static helper prototype function
// DPI Imm + Reg private functions
static void dpi_arith(mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2);
// DPI Imm private functions
static void dpi_imm_wide(instruction_t instruction, mstate_t *state);
// DPI Reg private functions
static OpGroup get_op_group(int m, uint32_t opr);
static void dpi_reg_arith_bitlogic(instruction_t instruction, mstate_t *state, OpGroup opgroup);
static void dpi_reg_bitlogic(instruction_t instruction, mstate_t *state, int sf, uint64_t op2);
static void dpi_reg_mult(instruction_t instruction, mstate_t *state);

//instruction_t ins;
//mstate *statep;
//dpi_imm(ins, statep);
//	Executes Data Processing Instruction (Immediate) 
//	instruction 32-bit unsigned instruction word
//	state* pointer to the machine state
void dpi_imm(instruction_t instruction, mstate_t *state) { 

	if (instruction->dp_imm.opi == ARITH_OPI) {	
		dpi_arith(state, instruction->dp_imm.sf, instruction->dp_imm.opc, instruction->dp_imm.rd, instruction->dp_imm.arith.rn, instruction->dp_imm.arith.imm12);
	} else if (instruction->dp_imm.opi == WIDE_OPI) {
		dpi_imm_wide(instruction, state);
	} else {
		fprintf(stderr, "Error: invalid opi.\n");
		exit(EXIT_FAILURE); // failed
	}
	pc_inc(state);
}

// Helper

//mstate_t *statep;
//dpi_arith(state, sf, opc, rd_index, rn_index, op2);
//	helper function for dpi imm and dpi reg
//	if dpi imm, op2 is imm12
//	if dpi reg, op2 is Rm shifted (operand) many bits by the shift encoded in (shift)
static void dpi_arith(mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2) {
	if (rn_index == REG32) { return; } // encodes SP
	if (sf == 0) { op2 = (uint32_t)op2; }
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]); 
	uint64_t result = (opc <= 1)? rn_val + op2 : rn_val - op2;
	if (sf == 0) { result = (uint32_t)result; }
	if ((opc & 1) == 0) { // without flag
		if (rd_index == REG32) {
			return; // encodes SP
		}
		state->regs[rd_index] = result; // opc == 0 or 2
	} else { // with flag
		const int msb = (sf == 1)? 63 : 31;
		const bool result_N = ((result >> msb) & 1) == 1; // check if result is negative
		state->ps.N = result_N; // set N to the sign bit of result
		state->ps.Z = result == 0; // set Z if result is 0
		const bool rn_N = ((rn_val >> msb) & 1) == 1; // check if rn_val is negative
		const bool op2_N = ((op2 >> msb) & 1) == 1; // check if op2 is negative
		if (opc == 1) { // addition, opc == 1
			state->ps.C = result < op2;
			state->ps.V = (!result_N && rn_N && op2_N) || (result_N && !rn_N && !op2_N);
		} else { // subtraction, opc == 3
			state->ps.C = rn_val >= op2;
			state->ps.V = (result_N && !rn_N && op2_N) || (!result_N && rn_N && !op2_N);
		}
		if (rd_index == REG32) {
			return; // encodes ZR
		}
		state->regs[rd_index] = result;
	}
}

//return dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);
//	helper function to perform wide functions based on opc if opi == WMOVE
static void dpi_imm_wide(instruction_t instruction, mstate_t *state) {
	int hw = instruction->dp_imm.wide.hw;
	uint64_t imm16 = instruction->dp_imm.wide.imm16;
	int sf = instruction->dp_imm.sf;
	int opc = instruction->dp_imm.opc;
	int rd_index = instruction->dp_imm.rd;
	
	if (sf == 0 && hw > 1) {
		fprintf(stderr, "Error: cannot shift by more than 16 bits in W-register.\n");
		exit(EXIT_FAILURE);
	}
	imm16 <<= (hw * 16);
	uint64_t rd_val = 0;
	switch(opc) { 
		case 0: //movn
			state->regs[rd_index] = (sf == 1)? ~imm16: ~((uint32_t)imm16);
			break;
		case 2: //movz 
			state->regs[rd_index] = imm16;
			break;
		case 3: //movk
			rd_val = state->regs[rd_index];
			rd_val = rd_val & ~((uint64_t)MASK(16) << (hw * 16));
			rd_val = rd_val | imm16;
			state->regs[rd_index] = (sf == 1)? rd_val : (uint32_t)rd_val;
			break;
		default:
			fprintf(stderr, "Error: invalid opc.\n");
			exit(EXIT_FAILURE);
	} 
}

//instruction_t ins;
//mstate_t *statep;
//dpi_reg(ins, statep);
//	Executes Data Processing Instruction (Register)
//	instruction 32-bit unsigned instruction 
//	state* pointer to the machine state
void dpi_reg(instruction_t instruction, mstate_t *state) {
	OpGroup opgroup = get_op_group(instruction->dp_reg.M, instruction->dp_reg.opr);
	if (opgroup == ARITHMETIC || opgroup == BIT_LOGIC) {
		dpi_reg_arith_bitlogic(instruction, state, opgroup);
	} else if (opgroup == MULTIPLY) {
		dpi_reg_mult(instruction, state);
	} else {
		fprintf(stderr, "Error: invalid OpGroup.\n");
		exit(EXIT_FAILURE); // opgroup UNKNOWN
	}
	pc_inc(state);
}

//OpGroup opgroup = get_op_group(m, opr);
//	helper function for dpi_reg to determine type of data processing instruction
static OpGroup get_op_group(int m, uint32_t opr) {
	if (m == ARITH_BL_M) {
		if ((opr >> 3) == 0) { // check that opr is 0xxx
			return BIT_LOGIC;
		}
		if ((opr & ARITH_OPR_MASK) == ARITH_OPR) { // check that opr is 1xx0
			return ARITHMETIC;
		}
	} else {
		if (opr == MUL_OPR) { // check that opr is 1000
			return MULTIPLY;
		}
	}
	return UNKNOWN;
}

//instruction_t ins;
//mstate_t *statep;
//OpGroup opgroup;
//dpi_reg_arith_bitlogic(ins, statep, opgroup)
//	helper function for dpi_reg to do arithmetic/bit-logic instructions
static void dpi_reg_arith_bitlogic(instruction_t instruction, mstate_t *state, OpGroup opgroup) {
	int shift = instruction->dp_reg.arith_logic.shift;
	int operand = instruction->dp_reg.arith_logic.imm6;
	int rm_index = instruction->dp_reg.rm; 
	int sf = instruction->dp_reg.sf;
	uint64_t rm_val = state->regs[rm_index];
	if (rm_index == REG32) { rm_val = 0; } // read ZR
	if (sf == 0) { rm_val = (uint32_t)rm_val; }
	uint64_t op2 = 0;
	switch (shift) {
		case LSL_SHIFT:
			op2 = rm_val << operand;
			break;
		case LSR_SHIFT:
			op2 = rm_val >> operand;
			break;
		case ASR_SHIFT:
			op2 = (sf == 1)? (((int64_t)rm_val) >> operand) : (((int32_t)rm_val) >> operand);
			break;
		case ROR_SHIFT:
			if (opgroup != BIT_LOGIC) {
				fprintf(stderr, "Error: cannot do rotate right for arithmetic instruction.\n");
				exit(EXIT_FAILURE); // invalid, no ror for arithmetic
			}
			op2 = (sf == 1)? (rm_val >> operand) | (rm_val << (64 - operand)) : (((uint32_t)rm_val) >> operand) | (((uint32_t)rm_val) << (32 - operand));
			break; 
		default:
			fprintf(stderr, "Error: invalid shift.\n");
			exit(EXIT_FAILURE); // invalid shift
	}
	if (opgroup == ARITHMETIC) {
		dpi_arith(state, sf, instruction->dp_reg.opc, instruction->dp_reg.rd, instruction->dp_reg.rn, op2); 
	} else if (opgroup == BIT_LOGIC) {
		dpi_reg_bitlogic(instruction, state, sf, op2); 
	} else {
		fprintf(stderr, "Error: invalid OpGroup.\n");
		exit(EXIT_FAILURE); //invalid opgroup
	}
}

//instruction_t ins;
//mstate_t *statep;
//dpi_reg_bitlogic(ins, statep, sf, op2);
//	helper function for dpi_reg_arith_bitlogic to do bit-logic instructions
static void dpi_reg_bitlogic(instruction_t instruction, mstate_t *state, int sf, uint64_t op2) {
	int n = instruction->dp_reg.arith_logic.N;
	int opc = instruction->dp_reg.opc;
	int rd_index = instruction->dp_reg.rd;
	int rn_index = instruction->dp_reg.rn;
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]);
	if (rn_index == REG32) { rn_val = 0; } // read ZR
	uint64_t result = 0;
	switch (opc) {
		case 0: // and, bic
			if (n == 0) { // and
				result = rn_val & op2;
			} else { // bic
				result = rn_val & ~op2;
			} 
			break;
		case 1: // orr, orn
			if (n == 0) { // orr
				result = rn_val | op2;
			} else { // orn
				result = rn_val | ~op2;
			} 
			break;
		case 2: // eor, eon
			if (n == 0) { // eor
				result = rn_val ^ op2;
			} else { // eon
				result = rn_val ^ ~op2;
			} 
			break;
		case 3: // ands, bics
			if (n == 0) { // ands
				result = rn_val & op2;
			} else { // bics
				result = rn_val & ~op2;
			} 
			const int msb = (sf == 1)? 63 : 31;
			const bool result_N = ((result >> msb) & 1) == 1; // check if result is negative	
			state->ps.N = result_N; // set N to the sign bit of result
			state->ps.Z = result == 0; // set Z if result is 0
			state->ps.C = 0;
			state->ps.V = 0;
			break;
		default:
			fprintf(stderr, "Error: Invalid opc.\n");
			exit(EXIT_FAILURE); // invalid opc
	}
	if (rd_index == REG32) { return; } // write to ZR
	if (sf == 0) { result = (uint32_t)result; }
	state->regs[rd_index] = result;
}

//instruction_t ins;
//mstate_t *statep;
//dpi_reg_mult(ins, statep);
//	helper function for dpi_reg to do multiply instructions
static void dpi_reg_mult(instruction_t instruction, mstate_t *state) {
	int sf = instruction->dp_reg.sf;
	int opc = instruction->dp_reg.opc;
	int rd_index = instruction->dp_reg.rd;
	int rn_index = instruction->dp_reg.rn;
	int rm_index = instruction->dp_reg.rm;
	int x = instruction->dp_reg.mul.x;
	int ra_index = instruction->dp_reg.mul.ra;
	
	assert(opc == 0);
	if (rd_index == REG32) { return; } // write to ZR
	uint64_t ra_val = (sf == 1)? state->regs[ra_index] : (uint32_t)(state->regs[ra_index]);
	if (ra_index == REG32) { ra_val = 0; } // read ZR
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]);
	if (rn_index == REG32) { rn_val = 0; } // read ZR
	uint64_t rm_val = (sf == 1)? state->regs[rm_index] : (uint32_t)(state->regs[rm_index]);
	if (rm_index == REG32) { rm_val = 0; } // read ZR
	uint64_t result = 0;
	if (x == 0) { // madd
		result = ra_val + (rn_val * rm_val); 
	} else { // x == 1, msub
		result = ra_val - (rn_val * rm_val);
	}
	if (sf == 0) { result = (uint32_t)result; }
	state->regs[rd_index] = result;	
}


