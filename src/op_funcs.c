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

static void parse_dp (
	TokenisedLine tl, 
	uint32_t *addr, 
	instruction_t ins, 
	int opc,
	int opr, 
	int N
) {
	if (operand_starts_with(tl, 2, '#')) { // dpi imm
		ins->op0 = DPI_OP0;
		ins->dp_imm.sf = operand_starts_with(tl, 0, 'x');
		ins->dp_imm.opc = opc;
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
		ins->dp_reg.opc = opc;
		ins->dp_reg.M = 0; // arith AND logic both uses 0
		ins->dp_reg.opr = opr;
		if (tl.num_operands == 5) { //shift 
			if (strcmp(tl.operands[3], "lsl") == 0) {
				ins->dp_reg.arith_logic.shift = LSL_SHIFT;
			} else if (strcmp(tl.operands[3], "lsr") == 0) {
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
		ins->dp_reg.arith_logic.N = N; // arith should not have N
	}
}

// op functions:
static void parseADD(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, ADD_OPC, ARITH_OPR, 0);
}

static void parseADDS(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, ADDS_OPC, ARITH_OPR, 0);
}

static void parseSUB(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, SUB_OPC, ARITH_OPR, 0);
}

static void parseSUBS(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, SUBS_OPC, ARITH_OPR, 0);
}

static void parseAND(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, AND_OPC, LOGIC_OPR, 0);
}

static void parseANDS(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, ANDS_OPC, LOGIC_OPR, 0);
}

static void parseBIC(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, BIC_OPC, LOGIC_OPR, 1);
}

static void parseBICS(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, BICS_OPC, LOGIC_OPR, 1);
}

static void parseEOR(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, EOR_OPC, LOGIC_OPR, 0);
}

static void parseEON(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, EON_OPC, LOGIC_OPR, 1);
}

static void parseORR(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, ORR_OPC, LOGIC_OPR, 0);
}

static void parseORN(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, addr, ins, ORN_OPC, LOGIC_OPR, 1);
}


	

static void parseMOVKNZ(
	TokenisedLine tl, 
	uint32_t *addr, 
	instruction_t ins, 
	int opc
) {
	ins->op0 = DPI_OP0;
	ins->dp_imm.sf = operand_starts_with(tl, 0, 'x');
	ins->dp_imm.opc = opc;
	ins->dp_imm.opi = WIDE_OPI;
	ins->dp_imm.rd = reg_index(tl.operands[0]);
	ins->dp_imm.wide.hw = tl.num_operands == 4 ? z_ext(tl.operands[3]) / 16 : 0;
	ins->dp_imm.wide.imm16 = z_ext(tl.operands[1]);
}

static void parseMOVK(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, addr, ins, MOVK_OPC);
}

static void parseMOVN(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, addr, ins, MOVN_OPC);
}

static void parseMOVZ(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, addr, ins, MOVZ_OPC);
}

	// B, BCOND, 
	// LDR, STR, 
	// HALT, 
	// INTDIR,
	// ALIAS

static void parseMUL(TokenisedLine tl, uint32_t *addr, instruction_t ins, int x) {
	ins->op0 = DPR_OP0;
	ins->dp_reg.rd = reg_index(tl.operands[0]);
	ins->dp_reg.rn = reg_index(tl.operands[1]);
	ins->dp_reg.rm = reg_index(tl.operands[2]);
	ins->dp_reg.sf = operand_starts_with(tl, 0, 'x');
	ins->dp_reg.opc = MUL_OPC;
	ins->dp_reg.M = 1; // multiply
	ins->dp_reg.opr = MUL_OPR;
	ins->dp_reg.mul.x = x;
	ins->dp_reg.mul.ra = reg_index(tl.operands[3]);
}

static void parseMADD(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMULL(tl, addr, ins, 0);
}

static void parseMSUB(TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMULL(tl, addr, ins, 1);
}



// array of function pointers
op_func_ptr op_func_arr[] = {
    [ADD] = &parseADD,
	[ADDS] = &parseADDS,
	[SUB] = &parseSUB,
	[SUBS] = &parseSUBS,
	[AND] = &parseAND,
	[ANDS] = &parseANDS,
	[BIC] = &parseBIC,
	[BICS] = &parseBICS,
	[EOR] = &parseEOR,
	[EON] = &parseEON,
	[ORR] = &parseORR,
	[ORN] = &parseORN,
	[MOVK] = &parseMOVK,
	[MOVN] = &parseMOVN,
	[MOVZ] = &parseMOVZ,
	[MADD] = &parseMADD,
	[MSUB] = &parseMSUB

};

