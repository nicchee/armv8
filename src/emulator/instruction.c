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

// Data Processing Instruction (Register)
// Position of each instruction part
#define SHIFT_OPERAND 10
#define SHIFT_RM 16
#define SHIFT_OPR 21
#define SHIFT_M 28
#define SHIFT_NEGATED 21
#define SHIFT_SHIFT 22
#define SHIFT_RA 10
#define SHIFT_X 15

// Branch Instruction
// Position of each instruction part
#define SHIFT_SIMM26 0
#define SHIFT_XN 5
#define SHIFT_COND 0
#define SHIFT_SIMM19 5

// Type of Branches
// Mask
#define MASKUNCONDOP 0xfc000000
#define MASKREGOP    0xffff0000
#define MASKCONDOP   0xfc000000
// Bit code
#define UNCONDOP  0x14000000
#define REGOP     0xd61f0000
#define CONDOP    0x54000000

// Single Data Transfer
// Position of each instruction part
#define SHIFT_RT 0
#define SHIFT_XN 5
#define SHIFT_SIMM19 5
#define SHIFT_OFFSET 10
#define SHIFT_L 22
#define SHIFT_U 24
#define SHIFT_SF 30
#define SHIFT_OP 31
#define SHIFT_XM 6

// Single Data Transfer
// Width of each instruction part
#define WIDTH_RT 5
#define WIDTH_XN 5
#define WIDTH_SIMM19 19
#define WIDTH_OFFSET 12
#define WIDTH_L 1
#define WIDTH_U 1
#define WIDTH_SF 1
#define WIDTH_OP 1
#define WIDTH_XM 5

static instruction_t dpi_imm_makeins(uint32_t ins, uint32_t op0);
static instruction_t dpi_reg_makeins(uint32_t ins, uint32_t op0);
static instruction_t branch_makeins(uint32_t ins, uint32_t op0);


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
	} else {
		fprintf(stderr, "opi is unknown");
		exit(EXIT_FAILURE);
	}

	return result;
}

static instruction_t dpi_reg_makeins(uint32_t ins, uint32_t op0) {
	instruction_t result;

	result->op0 = op0;
	result->dp_reg.rd = (ins >> SHIFT_RD) & MASK(5);	
	result->dp_reg.rn = (ins >> SHIFT_RN) & MASK(5);	
	result->dp_reg.rm = (ins >> SHIFT_RM) & MASK(5);	
	result->dp_reg.sf = (ins >> SHIFT_SF) & MASK(1);	
	result->dp_reg.opc = (ins >> SHIFT_OPC) & MASK(2);	
	result->dp_reg.m = (ins >> SHIFT_M) & MASK(1);	
	result->dp_reg.opr = (ins >> SHIFT_OPR) & MASK(4);

	if (op0 == DPR_OP0) {
		result->dp_reg.arith_logic.shift = (ins >> SHIFT_SHIFT) & MASK(2);
		result->dp_reg.arith_logic.imm6 = (ins >> SHIFT_OPERAND) & MASK(6);
		result->dp_reg.arith_logic.N = (ins >> SHIFT_NEGATED) & MASK(1);
	} else if (op0 == DPR_MUL_OP0) {
		result->dp_reg.mul.x = (ins >> SHIFT_X) & MASK(1);
		result->dp_reg.mul.ra = (ins >> SHIFT_RA) & MASK(5);
	} else {
		fprintf(stderr, "op0 is unknown");
		exit(EXIT_FAILURE);
	}

	return result;
}


static instruction_t branch_makeins(uint32_t ins, uint32_t op0) {
	instruction_t result;

	result->op0 = op0;
	if ((ins & MASKUNCONDOP) == UNCONDOP) {
		result->branch.type = UNCOND;
		result->branch.simm26 = (ins >> SHIFT_SIMM26) & MASK(26);
	} else if ((ins & MASKREGOP) == REGOP) {
		result->branch.type = REG;
		result->branch.xn = (ins >> SHIFT_XN) & MASK(5);
	} else if ((ins & MASKCONDOP) == CONDOP) {
		result->branch.type = COND;
		result->branchi.cond_t.cond = (ins >> SHIFT_COND) & MASK(4);
		result->branch.cond_t.simm19 = (ins >> SHIFT_SIMM19) & MASK(19);
	} else {
		fprintf(stderr, "Branch instruction not recognised");
		exit(EXIT_FAILURE);
	}
	
	return result;
}

static instruction_t load_store_makeins(uint32_t ins, uint32_t op0) {
	instruction_t result;

	result->op0 = op0;
	result->load_store.rt = (instruction >> SHIFT_RT) & MASK(WIDTH_RT); 
	result->load_store.sf = (instruction >> SHIFT_SF) & MASK(WIDTH_SF);
	result->load_store.sdt.xn = (instruction >> SHIFT_XN) & MASK(WIDTH_XN);
	result->load_store.sdt.L = (instruction >> SHIFT_L) & MASK(WIDTH_L);
	result->load_store.sdt.unsigned_offset.U = (instruction >> SHIFT_U) & MASK(WIDTH_U);
	uint32_t offset = (instruction >> SHIFT_OFFSET) & MASK(WIDTH_OFFSET);
	uint32_t bit31 = (instruction >> SHIFT_OP) & MASK(WIDTH_OP);	
	
	if (bit31 == 1) {
		if (result->load_store.sdt.unsigned_offset.U = 1) {
			// Unsigned Offset
			result->load_store.mode = UNSIGNED_OFFSET;
		} else {
			uint32_t det_bit = offset & 1;
			if (det_bit == 0) {
				// Register Offset
				result->load_store.mode = REG_OFFSET;
				uint32_t xm = (result->load_store. >> SHIFT_XM) & MASK(WIDTH_XM);
			} else {
				// Pre/Post Index
				result->load_store.mode = PRE_POST_INDEX;
				result->load_store.pre_post_index.simm9 = sgx((offset >> 2), 9);
				result->load_store.pre_post_index.I = (offset >> 1) & 1;
			}
		}
	} else if (bit31 == 0) {
		// Load Literal
		result->load_store.mode = LIT_LOAD;
		result->load_store.simm19 = (instruction >> SHIFT_SIMM19) & MASK(WIDTH_SIMM19);
	} else {
		fprintf(stderr, "ERROR: Load Store instruction not recognised");
		exit(EXIT_FAILURE);
	}

	return result;
}
