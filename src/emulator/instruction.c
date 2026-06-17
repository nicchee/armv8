#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "instruction.h"
#include "machine_state.h"

static instruction_t dpi_imm_makeins(uint32_t ins, uint32_t op0);
static instruction_t dpi_reg_makeins(uint32_t ins, uint32_t op0);
static instruction_t branch_makeins(uint32_t ins, uint32_t op0);
static instruction_t load_store_makeins(uint32_t ins, uint32_t op0);
static int64_t sgx(int32_t value, int bit_no);
static instruction_t result;

void init_ins(void) {
	result = malloc(sizeof(struct instruction_t));
	assert (result != NULL); 
}


instruction_t makeins(uint32_t ins) {
	uint32_t op0 = (ins >> SHIFT_OP0) & MASK(4);

	if ((op0 & MASK1110) == DPI_IMM) { // checks 100x
		return dpi_imm_makeins(ins, op0);
	} else if ((op0 & MASK0111) == DPI_REG) { // checks x101 
		return dpi_reg_makeins(ins, op0);
	} else if ((op0 & MASK0101) == LOADSTORE) { // checks x1x0
		return load_store_makeins(ins, op0);
	} else if ((op0 & MASK1110) == BRANCH) { // checks 101x
		return branch_makeins(ins, op0);
	} else {
		fprintf(stderr, "op0 is unknown");
		exit (EXIT_FAILURE);
	}
}

static instruction_t dpi_imm_makeins(uint32_t ins, uint32_t op0) {

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

	result->op0 = op0;
	result->dp_reg.rd = (ins >> SHIFT_RD) & MASK(5);	
	result->dp_reg.rn = (ins >> SHIFT_RN) & MASK(5);	
	result->dp_reg.rm = (ins >> SHIFT_RM) & MASK(5);	
	result->dp_reg.sf = (ins >> SHIFT_SF) & MASK(1);	
	result->dp_reg.opc = (ins >> SHIFT_OPC) & MASK(2);	
	result->dp_reg.M = (ins >> SHIFT_M) & MASK(1);	
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

	result->op0 = op0;
	if ((ins & MASKUNCONDOP) == UNCONDOP) {
		result->branch.type = UNCOND;
		result->branch.simm26 = (ins >> SHIFT_SIMM26) & MASK(26);
	} else if ((ins & MASKREGOP) == REGOP) {
		result->branch.type = REG;
		result->branch.xn = (ins >> SHIFT_XN) & MASK(5);
	} else if ((ins & MASKCONDOP) == CONDOP) {
		result->branch.type = COND;
		result->branch.cond_t.cond = (ins >> SHIFT_COND) & MASK(4);
		result->branch.cond_t.simm19 = (ins >> SHIFT_SIMM19) & MASK(19);
	} else {
		fprintf(stderr, "Branch instruction not recognised");
		exit(EXIT_FAILURE);
	}
	
	return result;
}

static instruction_t load_store_makeins(uint32_t ins, uint32_t op0) {

	result->op0 = op0;
	result->load_store.rt = (ins >> SHIFT_RT) & MASK(WIDTH_RT); 
	result->load_store.sf = (ins >> SHIFT_SDT_SF) & MASK(WIDTH_SF);
	result->load_store.sdt.xn = (ins >> SHIFT_XN) & MASK(WIDTH_XN);
	result->load_store.sdt.L = (ins >> SHIFT_L) & MASK(WIDTH_L);
	result->load_store.sdt.unsigned_offset.U = (ins >> SHIFT_U) & MASK(WIDTH_U);
	uint32_t offset = (ins >> SHIFT_OFFSET) & MASK(WIDTH_OFFSET);
	uint32_t bit31 = (ins >> SHIFT_OP) & MASK(WIDTH_OP);	
	
	if (bit31 == 1) {
		if (result->load_store.sdt.unsigned_offset.U == 1) {
			// Unsigned Offset
			result->load_store.mode = UNSIGNED_OFFSET;
			result->load_store.sdt.unsigned_offset.imm12 = offset;
		} else {
			uint32_t det_bit = offset & 1;
			if (det_bit == 0) {
				// Register Offset
				result->load_store.mode = REG_OFFSET;
				result->load_store.sdt.xm = (ins >> SHIFT_XM) & MASK(WIDTH_XM);
			} else {
				// Pre/Post Index
				result->load_store.mode = PRE_POST_INDEX;
				result->load_store.sdt.pre_post_index.simm9 = sgx((offset >> 2), 9);
				result->load_store.sdt.pre_post_index.I = (offset >> 1) & 1;
			}
		}
	} else if (bit31 == 0) {
		// Load Literal
		result->load_store.mode = LIT_LOAD;
		result->load_store.simm19 = (ins >> SHIFT_SIMM19) & MASK(WIDTH_SIMM19);
	} else {
		fprintf(stderr, "ERROR: Load Store instruction not recognised");
		exit(EXIT_FAILURE);
	}

	return result;
}

static int64_t sgx(int32_t value, int bit_no) {
  int64_t result = value & MASK(bit_no);
  
	if ((result >> (bit_no - 1)) == 1) {
    result |= ~(uint64_t)MASK(bit_no);
  }
  return result;
}

void free_ins(void){ 
	free(result);
}
