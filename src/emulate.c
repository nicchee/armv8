#include <stdlib.h>
#include "emulate.h"

// Data Processing Instruction (Register)

// Position of each instruction part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_OPERAND 10
#define SHIFT_RM 16
#define SHIFT_OPR 21
#define SHIFT_M 28
#define SHIFT_OPC 29
#define SHIFT_SF 31

// Arithmetic and Logical
#define ARITH_OPR_MASK 9
#define ARITH_OPR 8
#define ARITH_BL_M 0
#define SHIFT_NEGATED 21
#define SHIFT_SHIFT 22
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

// Multiply
#define OPR_MULT 8
#define SHIFT_RA 10
#define SHIFT_X 15

// Mask
#define MASK(x) ((1<<x) - 1)

// Special Register
#define REG32 31

typedef enum {
    UNKNOWN,
    ARITHMETIC,
    BIT_LOGIC,
    MULTIPLY
} OpGroup;

extern OpGroup get_op_group(int m, uint32_t opr);
extern int dpi_reg(uint32_t instruction, mstate_t *state);
extern int dpi_reg_arith_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index, OpGroup opgroup);
extern int dpi_reg_arith(mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2);
extern int dpi_reg_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2);
extern int dpi_reg_mult(uint64_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index);

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

/*
 * @brief Executes Data Processing Instruction (Register)
 * @param instruction 32-bit unsigned instruction 
 * @param state* pointer to the machine state
 * @return 0 if program exits successfully; 1 if program failed
 */
int dpi_reg(uint32_t instruction, mstate_t *state) {
	int rd_index = (instruction >> SHIFT_RD) & MASK(5);
	int rn_index = (instruction >> SHIFT_RN) & MASK(5);
	int rm_index = (instruction >> SHIFT_RM) & MASK(5);
	int sf = (instruction >> SHIFT_SF) & MASK(1);
	int opc = (instruction >> SHIFT_OPC) & MASK(2);
	int m = (instruction >> SHIFT_M) & MASK(1);
	uint32_t opr = (instruction >> SHIFT_OPR) & MASK(4);
	OpGroup opgroup = get_op_group(m, opr);
	if (opgroup == ARITHMETIC || opgroup == BIT_LOGIC) {
		return dpi_reg_arith_bitlogic(instruction, state, sf, opc, rd_index, rn_index, rm_index, opgroup);
	} else if (opgroup == MULTIPLY) {
		return dpi_reg_mult(instruction, state, sf, opc, rd_index, rn_index, rm_index);
	} else {
		exit(1); // opgroup UNKNOWN
	}
}

/*
 * helper function for dpi_reg to determine type of data processing instruction
 */
static OpGroup get_op_group(int m, uint32_t opr) {
	if (m == ARITH_BL_M) {
		if ((opr >> 3) == 0) { // check that opr is 0xxx
			return BIT_LOGIC;
		}
		if ((opr & ARITH_OPR_MASK) == ARITH_OPR) { // check that opr is 1xx0
			return ARITHMETIC;
		}
	} else {
		if (opr == OPR_MULT) { // check that opr is 1000
			return MULTIPLY;
		}
	}
	return UNKNOWN;
}

/*
 * helper function for dpi_reg to do arithmetic/bit-logic instructions
 */
static int dpi_reg_arith_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index, OpGroup opgroup) {
	int shift = (instruction >> SHIFT_SHIFT) & MASK(2);
	int operand = (instruction >> SHIFT_OPERAND) & MASK(6); 
	uint64_t rm_val = state->regs[rm_index];
	if (rm_index == REG32) { rm_val = 0; }
	uint64_t op2 = 0;
	switch (shift) {
		case LSL:
			uint64_t result = rm_val << shift;
			op2 = (sf == 1)? result : (uint32_t)result;
			break;
		case LSR:
			uint64_t result = rm_val >> shift;
			op2 = (sf == 1)? result : (uint32_t)result;
			break;
		case ASR:
			op2 = (sf == 1)? (((int64_t)rm_val) >> shift) : (((int32_t)rm_val) >> shift);
			break;
		case ROR:
			if (opgroup != BIT_LOGIC) {
				return 1; // invalid, no ror for arithmetic
			}
			op2 = (sf == 1)? (rm_val >> shift) | (rm_val << (64 - shift)) : (((uint32_t)rm_val) >> shift) | (((uint32_t)rm_val) << (32 - shift));
			break; 
		default:
			exit(1); // invalid shift
	}
	if (opgroup == ARITHMETIC) {
		return dpi_reg_arith(state, sf, opc, rd_index, rn_index, op2); 
	} else if (opgroup == BIT_LOGIC) {
		return dpi_reg_bitlogic(instruction, state, sf, opc, rd_index, rn_index, op2); 
	} else {
		exit(1); //invalid opgroup
	}
}

/*
 * helper function for dpi_reg_arith_bitlogic to do arithmetic instructions
 */
static int dpi_reg_arith(mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2) {
	if (rn_index == REG32) { return 0; } // encodes SP
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]); 
	uint64_t result = (opc <= 1)? rn_val + op2 : rn_val - op2;
	if (sf == 0) { result = (uint32_t)result; }
	if ((opc & 1) == 0) { // without flag
		if (rd_index == REG32) {
			return 0; // encodes SP
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
			return 0; // encodes ZR
		}
		state->regs[rd_index] = result;
	}
	return 0;
}

/*
 * helper function for dpi_reg_arith_bitlogic to do bit-logic instructions
 */
static int dpi_reg_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, uint64_t op2) {
	int n = (instruction >> SHIFT_NEGATED) & MASK(1);
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]);
	if (rn_index == REG32) { rn_val = 0; } // read ZR
	uint64_t result = 0;
	switch (opc) {
		case 0: // and, bic
			if (n == 0) { // and
				result = rn_val & op2;
			} else if (n == 1) { // bic
				result = rn_val & ~op2;
			} else {
				exit(1); // invalid n
			}
			break;
		case 1: // orr, orn
			if (n == 0) { // orr
				result = rn_val | op2;
			} else if (n == 1) { // orn
				result = rn_val | ~op2;
			} else {
				exit(1); // invalid n
			}
			break;
		case 2: // eor, eon
			if (n == 0) { // eor
				result = rn_val ^ op2;
			} else if (n == 1) { // eon
				result = rn_val ^ ~op2;
			} else {
				exit(1); // invalid n
			}
			break;
		case 3: // ands, bics
			if (n == 0) { // ands
				result = rn_val & op2;
			} else if (n == 1) { // bics
				result = rn_val & ~op2;
			} else {
				exit(1); // invalid n
			}
			const int msb = (sf == 1)? 63 : 31;
			const bool result_N = ((result >> msb) & 1) == 1; // check if result is negative	
			state->ps.N = result_N; // set N to the sign bit of result
			state->ps.Z = result == 0; // set Z if result is 0
			state->ps.C = 0;
			state->pc.V = 0;
			break;
		default:
			exit(1); // invalid opc
	}
	if (rd_index == REG32) { return 0; } // write to ZR
	if (sf == 0) { result = (uint32_t)result; }
	state->regs[rd_index] = result;
	return 0;
}

/*
 * helper function for dpi_reg to do multiply instructions
 */
static int dpi_reg_mult(uint64_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index) {
	assert(opc == 0);
	if (rd_index == REG32) { return 0; } // write to ZR
	int x = (instruction >> SHIFT_X) & MASK(1);
	int ra_index = (instruction >> SHIFT_RA) & MASK(5);
	uint64_t ra_val = (sf == 1)? state->regs[ra_index] : (uint32_t)(state->regs[ra_index]);
	if (ra_index == REG32) { ra_val = 0; } // read ZR
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]);
	if (rn_index == REG32) { rn_val = 0; } // read ZR
	uint64_t rm_val = (sf == 1)? state->regs[rm_index] : (uint32_t)(state->regs[rm_index]);
	if (rm_index == REG32) { rm_val = 0; } // read ZR
	if (x == 0) { // madd
		state->regs[rd_index] = ra_val + (rn_val * rm_val); 
	} else { // x == 1, msub
		state->regs[rd_index] = ra_val - (rn_val * rm_val);
	}
	return 0;
}

