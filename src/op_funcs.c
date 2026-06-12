#include "op_funcs.h"
#include "ins_parser.h"
#include "symtable.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define NUM_OPFUNCS 21

//if (operand_starts_with(tl, index, c)) {..}
//	return true if operand at index start with c
static int operand_starts_with(TokenisedLine tl, int index, char c) {
	return *(tl.operands[index]) == c;
}

static bool compare_string(char *s1, char *s2) {
	return (strcmp(s1, s2) == 0) ? true : false;  
}

static uint8_t reg_index(char *reg) {
	return atoi(reg + 1);
} 

//	sign extend an immediate value that takes the form of "#_"
static int32_t s_ext(char *imm, int bits) {
	int imm_val = atoi(imm + 1);
	int shift = 32 - bits;

	//shift value to MSB
	//typecase
	//logical right shift
	return ((int32_t)(imm_val << shift)) >> shift; 
}

//	zero extend an immediate value that takes the form of "#_"
static uint32_t z_ext(char *imm) {
	int imm_val = atoi(imm + 1); //remove '#'

	return 0xffff & imm_val;
}

static int32_t get_offset(uint32_t curraddr, char *lit, symtable table) {
	int32_t targetaddr = (*lit == '#') ? // first character of literal is # (immediate value)
			s_ext(lit, 26) : get_addr(table, lit);

	return (targetaddr - curraddr) / 4;
}

static void parse_dp (
	TokenisedLine tl, 
	instruction_t ins, 
	int opc,
	int opr, 
	int N
) {
	ins->int_dir.is_label = false;
	if (operand_starts_with(tl, 2, '#')) { // dpi imm
		ins->op0 = DPI_OP0;
		ins->dp_imm.sf = operand_starts_with(tl, 0, 'x');
		ins->dp_imm.opc = opc;
		ins->dp_imm.opi = ARITH_OPI;
		ins->dp_imm.rd = reg_index(tl.operands[0]);
		ins->dp_imm.arith.rn = reg_index(tl.operands[1]);
		ins->dp_imm.arith.sh = tl.num_operands == 5 && (compare_string(tl.operands[4], "#12"));  // sh == 1 (lsl imm by 12)
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
static void parseADD(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, ADD_OPC, ARITH_OPR, 0);
}

static void parseADDS(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, ADDS_OPC, ARITH_OPR, 0);
}

static void parseSUB(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, SUB_OPC, ARITH_OPR, 0);
}

static void parseSUBS(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, SUBS_OPC, ARITH_OPR, 0);
}

static void parseAND(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, AND_OPC, LOGIC_OPR, 0);
}

static void parseANDS(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, ANDS_OPC, LOGIC_OPR, 0);
}

static void parseBIC(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, BIC_OPC, LOGIC_OPR, 1);
}

static void parseBICS(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, BICS_OPC, LOGIC_OPR, 1);
}

static void parseEOR(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, EOR_OPC, LOGIC_OPR, 0);
}

static void parseEON(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, EON_OPC, LOGIC_OPR, 1);
}

static void parseORR(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl, ins, ORR_OPC, LOGIC_OPR, 0);
}

static void parseORN(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_dp (tl,ins, ORN_OPC, LOGIC_OPR, 1);
}
	

static void parseMOVKNZ(
	TokenisedLine tl, 
	instruction_t ins, 
	int opc
) {
	ins->int_dir.is_label = false;
	ins->op0 = DPI_OP0;
	ins->dp_imm.sf = operand_starts_with(tl, 0, 'x');
	ins->dp_imm.opc = opc;
	ins->dp_imm.opi = WIDE_OPI;
	ins->dp_imm.rd = reg_index(tl.operands[0]);
	ins->dp_imm.wide.hw = tl.num_operands == 4 ? z_ext(tl.operands[3]) / 16 : 0;
	ins->dp_imm.wide.imm16 = z_ext(tl.operands[1]);
}

static void parseMOVK(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, ins, MOVK_OPC);
}

static void parseMOVN(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, ins, MOVN_OPC);
}

static void parseMOVZ(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMOVKNZ(tl, ins, MOVZ_OPC);
}

static void parseMUL(TokenisedLine tl, uint32_t *addr, instruction_t ins, int x) {
	ins->int_dir.is_label = false;
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

static void parseMADD(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMUL(tl, addr, ins, 0);
}

static void parseMSUB(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parseMUL(tl, addr, ins, 1);
}

static void parse_B_COND (symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins, branch_type_t type) {
	ins->int_dir.is_label = false;
	ins->op0 = BRANCH_OP0;
	ins->branch.type = type;
	if (type == UNCOND) { // BRANCH UNCOND
		ins->branch.simm26 = get_offset(*addr, tl.operands[0], table); // offset 
	} else { // BRANCH COND
		ins->branch.cond_t.simm19 = s_ext(tl.operands[1], 19);
		char * cond = tl.opcode + 2;
		if (compare_string(cond, "eq")) {
			ins->branch.cond_t.cond = B_EQ;
		} else if (compare_string(cond, "ne")) {
			ins->branch.cond_t.cond = B_NE;
		} else if (compare_string(cond, "ge")) {
			ins->branch.cond_t.cond = B_GE;
		} else if (compare_string(cond, "lt")) {
			ins->branch.cond_t.cond = B_LT;
		} else if (compare_string(cond, "gt")) {
			ins->branch.cond_t.cond = B_GT;
		} else if (compare_string(cond, "le")) {
			ins->branch.cond_t.cond = B_LE;
		} else {
			ins->branch.cond_t.cond = B_AL;
		}
	}
}

static void parseB(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_B_COND(table, tl, addr, ins, UNCOND);
}

static void parseBCOND(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	parse_B_COND(table, tl, addr, ins, COND);
}

static void parseLS(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins ) {
	ins->int_dir.is_label = false;
	ins->op0 = LOAD_STORE_OP0;
	ins->load_store.rt = reg_index(tl.operands[0]);
	ins->load_store.sf = operand_starts_with(tl, 0, 'x');

	//Load Literal
	if (!operand_starts_with(tl, 1, '[')) { 
		ins->load_store.mode = LIT_LOAD;
		ins->load_store.simm19 = get_offset(*addr, tl.operands[1], table);
	}
	
	//SDT
	else { 
		// Shared SDT vars: L and xn
		ins->load_store.sdt.L = (strcmp(tl.opcode, "ldr") == 0)? 1: 0;
		char *xn = (ins->load_store.sf) ? // 1 -> x, 0 -> w
			strchr(tl.operands[1], 'x') : 
			strchr(tl.operands[1], 'w');
		assert (xn != NULL);
		ins->load_store.sdt.xn = reg_index(xn);
		
		// Pre/ Post Index
		if (((tl.operands[1])[strlen(tl.operands[1]) - 1]) != ']' ) { 
			ins->load_store.mode = PRE_POST_INDEX;
			char *simm = (tl.num_operands == 3) ?
				strchr(tl.operands[2], '#') : 
				strchr(tl.operands[1], '#');
			assert (simm != NULL);
			ins->load_store.sdt.pre_post_index.simm9 = s_ext(simm,9); 
			ins->load_store.sdt.pre_post_index.I = tl.num_operands == 2; // 1 -> pre, 0 -> post
		}
	
		// Reg offset: ldr rt, [xn, xm]
		else if ( (tl.num_operands == 3) && 
			( !operand_starts_with(tl, 2, '#') )) { 
			ins->load_store.mode = REG_OFFSET;
			ins->load_store.sdt.xm = reg_index(tl.operands[2]);
		}

		//Unsigned offset: ldr rt, [xn{, #<imm>}]
		else {
			ins->load_store.mode = UNSIGNED_OFFSET;
			ins->load_store.sdt.unsigned_offset.U = 1;
			char *imm = (tl.num_operands == 3) ?
				strchr(tl.operands[2], '#') : 
				NULL; 
			//if not given #<imm>, set imm to 0
			uint32_t parsed_imm = imm ? z_ext(imm) : 0;
			ins->load_store.sdt.unsigned_offset.imm12 = 
				parsed_imm / (ins->load_store.sf ? 8 : 4); 
		}

	} //endif for SDT
} //end of parseLS 

static void parseINTDIR(symtable table, TokenisedLine tl, uint32_t *addr, instruction_t ins) {
	if (isdigit(tl.operands[0][0]) || 
	tl.operands[0][0] == '-') { //number
		ins->int_dir.is_label = false;
		ins->int_dir.int_val = (uint32_t) strtol(tl.operands[0], NULL, 0);
		ins->int_dir.label = NULL;
	} else { //label
		ins->int_dir.is_label = true;
		ins->int_dir.int_val = 0;
		ins->int_dir.label = strdup(tl.operands[0]);
	}
	*addr += 4; //.int directive always takes up 4 bytes of memory
}

// array of function pointers
op_func_ptr op_func_arr[] = { 
    [ADD] = parseADD,
	[ADDS] = parseADDS,
	[SUB] = parseSUB,
	[SUBS] = parseSUBS,
	[AND] = parseAND,
	[ANDS] = parseANDS,
	[BIC] = parseBIC,
	[BICS] = parseBICS,
	[EOR] = parseEOR,
	[EON] = parseEON,
	[ORR] = parseORR,
	[ORN] = parseORN,
	[MOVK] = parseMOVK,
	[MOVN] = parseMOVN,
	[MOVZ] = parseMOVZ,
	[MADD] = parseMADD,
	[MSUB] = parseMSUB,
	[B] = parseB,
	[BCOND] = parseBCOND,
	[LOAD_STORE] = parseLS,
	[INTDIR] = parseINTDIR
};



