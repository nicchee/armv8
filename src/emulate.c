#include <stdlib.h>
#include "emulate.h"

// Data Processing Instructions




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
 * @brief: Executes Data Processing Instruction (Immediate) 
 *
 * @param instruction: 32-bit unsigned instruction word
 * @param state *: pointer to the machine state
 * @return: 0 if program exit successfully; 1 if program failed
 */
int dpi_imm(uint32_t instruction, mstate_t *state) { 
	uint32_t opi = (instruction >> SHIFT_OPI) & MASK(3);
	if (opi == ARITH ) {
		
	} else if (opi == WMOVE) {
	
	} else {
		return 1; // failed
	}
}


/*
 * @brief: DPI Helper Arithmetic operation
 *
 * @param instruction: 32-bit unsigned instruction word
 * @param state *: pointer to the machine state
 * @return: 0 if program exit successfully; 1 if program failed
 */
int dpi_imm_arith(uint32_t instruction, mstate_t *state) {
	uint32_t imm12 = (instruction >> SHIFT_IMM12) & MASK(12);
	int sh = (instruction >> SHIFT_SF) & MASK(1);
	if (sh == 1) {
		imm12 <<= 12;	
	}
	int opc = (instruction >> SHIFT_OPC) & MASK(2);
	int sf = (instruction >> SHIFT_SF) & MASK(1) // 0 for 32-bit; 1 for 64-bit
	int rn_index = (instruction >> SHIFT_RN) & MASK(5); 
	int rd_index = (instruction >> SHIFT_RD) & MASK(5);
	if (rd == REG32) {
		return 0;
	}
	int result; 
	switch (opc) 
	{
		case 0: //add
			if (rd == REG32){
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
				state->ps.Z = (((rd_val >> 63) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = rd_val == 0; // set Z flag if = 0
                state->ps.C = rd_val < imm12; // set C flag if result < summand (due too overflow)
                int64_t op1 = (int64_t)state->regs[rn_index];
                int64_t op2 = (int64_t)imm12
                state->ps.V = (rd_vap > 0 && op1<0 && op2<0) || (rd_val < 0 && op1>0 && op2>0)
				if (rd_index != REG32) { state->regs[rd_index] = rd_val; }
			} else {
			 	state->ps.Z = (((rd_val >> 31) & MASK(1)) == 1); // set N flag based on MSB
                state->ps.Z = wd_val == 0; // set Z flag if = 0
                state->ps.C = wd_val < imm12; // set C flag if result < summand (due too overflow)
                int32_t op1 = (int32_t)state->regs[rn_index];
                int32_t op2 = (int32_t)imm12
                state->ps.V = (wd_vap > 0 && op1<0 && op2<0) || (wd_val < 0 && op1>0 && op2>0)
				if (rd_index != REG32) { state->regs[rd_index] = (uint64_t)wd_val; }
			}
			break;
        case 2: //minus 
	}

	
	
}



