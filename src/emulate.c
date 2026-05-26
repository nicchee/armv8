#include <stdlib.h>
#include "emulate.h"

// Data Processing Instruction (Immedaite) #defines

// Position of each instruction part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_IMM12 10
#define SHIFT_SH 22
#define SHIFT_IMM16 5
#define SHIFT_HW 21
#define SHIFT_OPI 23
#define SHIFT_OPC 29
#define SHIFT_SF 31

// Mask Macro
#define MASK(x) ((1<<x) - 1)

// OPI instruction
#define ARITH 2 
#define WMOVE 5

// Special Register
#define REG32 31

extern dpi_imm(uint32_t instruction, mstate_t *state);

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

/*
 * @brief Executes Data Processing Instruction (Immediate) 
 * @param instruction 32-bit unsigned instruction word
 * @param state* pointer to the machine state
 * @return 0 if program exit successfully; 1 if program failed
 */
int dpi_imm(uint32_t instruction, mstate_t *state) { 
	int rd_index = (instruction >> SHIFT_RD) & MASK(5);
	uint32_t opi = (instruction >> SHIFT_OPI) & MASK(3);
	int sh = (instruction >> SHIFT_SF) & MASK(1);
	int opc = (instruction >> SHIFT_OPC) & MASK(2);
	if (opi == ARITH ) {
		if (dpi_imm_arith(instruction, state, sh, opc, rd_index) == 1) { return 1; }
		return 0;
	} else if (opi == WMOVE) {
	
	} else {
		return 1; // failed
	}
}


/*
 * @brief DPI Helper Arithmetic operation
 * @param instruction 32-bit unsigned instruction word
 * @param state pointer to the machine state
 * @param sh value of sh (32/64 bits operation)
 * @param opc type of operation in arithmetic operation
 * @param rd_index index of rd 
 * @return 0 if program exit successfully; 1 if program failed
 * @attention lots of redundant code
 */
int dpi_imm_arith(uint32_t instruction, mstate_t *state, int sh, int opc, int rd_index) {
	uint32_t imm12 = (instruction >> SHIFT_IMM12) & MASK(12);
	if (sh == 1) {
		imm12 <<= 12;	
	}
	int sf = (instruction >> SHIFT_SF) & MASK(1); // 0 for 32-bit; 1 for 64-bit
	int rn_index = (instruction >> SHIFT_RN) & MASK(5); 
	if (rd_index == REG32) {
		return 0;
	}
	int result; 
	switch (opc) 
	{
		case 0: //add
			if (rd_index == REG32){
				break;
			} 
			if (sf == 1) {
				state->regs[rd_index] = state->regs[rn_index] + imm12;
			} else {
				uint32_t wn = (uint32_t)state->regs[rn_index];
			 	state->regs[rd_index] =(uint64_t) (wn + imm12);
			}
			break;
		case 1: //add with flag
			uint64_t rd_val = state->regs[rn_index] + imm12;
			uint32_t wd_val = (uint32_t)state->regs[rn_index] + imm12;
			if (sf == 1) {
				state->ps.N = (((rd_val >> 63) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = rd_val == 0; // set Z flag if = 0
                state->ps.C = rd_val < imm12; // set C flag if result < summand (due too overflow)
                int64_t op1 = (int64_t)state->regs[rn_index];
                int64_t op2 = (int64_t)imm12;
                state->ps.V = (rd_val > 0 && op1<0 && op2<0) || (rd_val < 0 && op1>0 && op2>0);
				if (rd_index != REG32) { state->regs[rd_index] = rd_val; }
			} else {
			 	state->ps.N = (((rd_val >> 31) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = wd_val == 0; // set Z flag if = 0
                state->ps.C = wd_val < imm12; // set C flag if result < summand (due too overflow)
                int32_t op1 = (int32_t)state->regs[rn_index];
                int32_t op2 = (int32_t)imm12;
                state->ps.V = (wd_val > 0 && op1<0 && op2<0) || (wd_val < 0 && op1>0 && op2>0);
				if (rd_index != REG32) { state->regs[rd_index] = (uint64_t)wd_val; }
			}
			break;
        case 2: //minus 
			if (rd_index == REG32){
				break; // 
			} 
			if (sf == 1) {
				state->regs[rd_index] = state->regs[rn_index] - imm12;
			} else {
				uint32_t wn = (uint32_t)state->regs[rn_index];
			 	state->regs[rd_index] =(uint64_t) (wn - imm12);
			}
			break;
		case 3: //minus with flag
			uint64_t rd_val = state->regs[rn_index] - imm12;
			uint32_t wd_val = (uint32_t)state->regs[rn_index] - imm12;
			if (sf == 1) {
				state->ps.N = (((rd_val >> 63) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = rd_val == 0; // set Z flag if = 0
                state->ps.C = state->regs[rn_index] < imm12; // set C flag if result < summand (due too overflow)
                int64_t op1 = (int64_t)state->regs[rn_index];
                int64_t op2 = (int64_t)imm12;
                state->ps.V = (rd_val < 0 && op1>0 && op2<0) || (rd_val > 0 && op1<0 && op2>0); // if (+) - (-) = (-) or (-) - (+) = (+)
				if (rd_index != REG32) { state->regs[rd_index] = rd_val; }
			} else {
			 	state->ps.N = (((rd_val >> 31) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = wd_val == 0; // set Z flag if = 0
                state->ps.C = (uint32_t)state->regs[rn_index] < imm12; // set C flag if result < summand (due too overflow)
                int32_t op1 = (int32_t)state->regs[rn_index];
                int32_t op2 = (int32_t)imm12;
                state->ps.V = !(wd_val < 0 && op1>0 && op2<0) || (wd_val > 0 && op1<0 && op2>0); // if (+) - (-) = (-) or (-) - (+) = (+)
				if (rd_index != REG32) { state->regs[rd_index] = (uint64_t)wd_val; }
			}
			break;
		default: 
			return 1;
	}
}

/*
* @brief DPI Helper Wide Move operation
* @param instruction: 32-bit unsigned instruction word
* @param state *: pointer to the machine state
* @param sh value of sh (32/64 bits operation)
* @param opc type of operation in arithmetic operation
* @param rd_index index of rd 
* @return 0 if program exit successfully; 1 if program failed
* @attention lots of redundant code
*/
static int dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index) {
	int hw = ((instruction >> SHIFT_HW) & MASK(2));
	uint64_t imm16 = (instruction >> SHIFT_IMM16) & MASK(16);
	if (sf == 0 && hw > 1) {
		return 1;
	}
	imm16 <<= (hw * 16);
	switch(opc) { 
		case 0: //movn
			state->regs[rd_index] = (sf == 1)? ~imm16: ~((uint32_t)imm16);
			break;
		case 2: 
			state->regs[rd_index] = imm16;
			break;
		case 3: 
			uint64_t rd_val = state->regs[rd_index];
			rd_val = rd_val & ~(MASK(16) << (hw * 16));
			rd_val = rd_val | imm16;
			state->regs[rd_index] = (sf == 1)? rd_val : (uint32_t)rd_val;
		default:
			return 1;
		
	} 
}



