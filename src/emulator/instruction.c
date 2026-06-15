#include <stdint.h>

#include "instruction.h"

#define SHIFT_OP0 25
#define MASK1111 15

// Data Processing Instruction Immediate + Register
// Position of each instruction part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_OPC 29
#define SHIFT_SF 31

// Data Processing Instruction (Immediate)
// Position of each instruction part
#define SHIFT_IMM12 10
#define SHIFT_SH 22
#define SHIFT_IMM16 5
#define SHIFT_HW 21
#define SHIFT_OPI 23


instruction_t makeins(uint32_t ins) {
	uint32_t op0 = (ins >> SHIFT_OP0) & MASK1111;

	switch (op0) {
		case DPI_OP0:
			return dpi_imm_makeins(ins, op0);
		case DPR_OP0:
			return dpi_reg_makeins(ins, op0);
		case DPR_MUL_OP0:
			return dpi_reg_makeins(ins, op0);
		case BRANCH_OP0:
			return branch_makeins(ins, op0);
		case BRANCH_REG_OP0:
			return branch_makeins(ins, op0);
		case LOAD_STORE_OP0:
			return load_store_makeins(ins, op0);
		default:
			fprintf(stderr, "op0 is unknown");
			exit(EXIT_FAILURE);
	}
}

static instruction_t dpi_imm_makeins(uint32_t ins, uint32_t op0) {
	instruction_t result;

	result->op0 = op0;
	result->dp_imm.sf = (ins >> SHIFT_SF) & MASK(1);
	result->dp_imm.opi = (ins >> SHIFT_OPI) & MASK(3);
	result->dp_imm.opc = (ins >> SHIFT_OPC) & MASK(2);
	result->dp_imm.rd = (ins >> SHIFT_RD) & MASK(5);

	if (result->dp_imm.opi == ARITH_OPI) {
		result->dp_imm.arith.rn = (ins >> SHIFT_RN) & MASK(5);
		result->dp_imm.arith.sh = (ins >> SHIFT_SH) & MASK(1);
		result->dp_imm.arith.imm12 = (ins >> SHIFT_IMM12) & MASK(12);
		if (result->dp_imm.arith.sh == 1) {
			result->dp_imm.arith.imm12 <<= 12;
		}
	} else if (result->dp_imm.opi == WIDE_OPI) {
		result->dp_imm.wide.hw = (ins >> SHIFT_HW) & MASK(2);
		result->dp_imm.wide.imm16 = (ins >> SHIFT_IMM16) & MASK(16);
	}

	return result;
}

