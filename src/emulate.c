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

// Multiply
#define OPR_MULT 8
#define SHIFT_RA 10
#define SHIFT_X 15

// Mask
#define MASK(x) ((1<<x) - 1)

typedef enum {
    UNKNOWN,
    ARITHMETIC,
    BIT_LOGIC,
    MULTIPLY
} OpGroup;

extern OpGroup get_op_group(int m, uint32_t opr);
extern int dpi_reg(uint32_t instruction, mstate_t *state);

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

int dpi_reg(uint32_t instruction, mstate_t *state) {
	int sf = (instruction >> SHIFT_SF) & MASK(1);
	int opc = (instruction >> SHIFT_OPC) & MASK(2);
	int m = (instruction >> SHIFT_M) & MASK(1);
	uint32_t opr = (instruction >> SHIFT_OPR) & MASK(4);
	OpGroup opgroup = get_op_group(m, opr);
}

OpGroup get_op_group(int m, uint32_t opr) {
	if (m == ARITH_BL_M) {
		if ((opr >> 3) == 0) { //check that opr is 0xxx
			return BIT_LOGIC
		}
		if ((opr & ARITH_OPR_MASK) == ARITH_OPR) { //check that opr is 1xx0
			return ARITHMETIC
		}
	} else {
		if (opr == OPR_MULT) { //check that opr is 1000
			return MULTIPLY
		}
	}
}
