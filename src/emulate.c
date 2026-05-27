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
extern int dpi_reg_arith();
extern int dpi_reg_bitlogic();
extern int dpi_reg_mult();

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

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
		return dpi_reg_arith_bitlogic();
	} else if (opgroup == MULTIPLY) {
		return dpi_reg_mult();
	} else {
		return 1; // opgroup UNKNOWN
	}
}

OpGroup get_op_group(int m, uint32_t opr) {
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

int dpi_reg_arith_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index, OpGroup opgroup) {
	int shift = (instruction >> SHIFT_SHIFT) & MASK(2);
	int operand = (instruction >> SHIFT_OPERAND) & MASK(6);
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)(state->regs[rn_index]); 
	uint64_t rm_val = state->regs[rm_index];
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
	}
	if (opgroup == ARITHMETIC) {
		return dpi_reg_arith(instruction, state, sf, opc, rd_index, rn_index, rm_index); 
	} else if (opgroup == BIT_LOGIC) {
		return dpi_reg_bitlogic(instruction, state, sf, opc, rd_index, rn_index, rm_index); 
	} else {
		return 1; //invalid opgroup
	}
}

int dpi_reg_arith(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index) {

}

int dpi_reg_bitlogic(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index, int rn_index, int rm_index) {
	int shift = (instruction >> SHIFT_SHIFT) & MASK(2);
	int operand = (instruction >> SHIFT_OPERAND) & MASK(6);
	int n = (instruction >> SHIFT_NEGATED) & MASK(1);
	
}

