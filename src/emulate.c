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
#define MASK(x) ((1<<(x)) - 1)

// OPI instruction
#define ARITH 2 
#define WMOVE 5

// Special Register
#define REG32 31

extern int dpi_imm(uint32_t instruction, mstate_t *state);
extern int dpi_imm_arith(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);
extern int dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);

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
	int sf = (instruction >> SHIFT_SF) & MASK(1);
	int opc = (instruction >> SHIFT_OPC) & MASK(2);
	if (opi == ARITH ) {
		return dpi_imm_arith(instruction, state, sf, opc, rd_index);
	} else if (opi == WMOVE) {
		return dpi_imm_wide(instruction, state, sf, opc, rd_index);
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
static int dpi_imm_arith(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index) {
	if (opc > 3) {return 1;} // opc code not recognised
	uint32_t imm12 = (instruction >> SHIFT_IMM12) & MASK(12);
	int sh = (instruction >> SHIFT_SH) & MASK(1);
	if (sh == 1) {imm12 <<= 12;}
	int rn_index = (instruction >> SHIFT_RN) & MASK(5); 
	if (rn_index == REG32) {return 0;}
	uint64_t rn_val = (sf == 1)? state->regs[rn_index] : (uint32_t)state->regs[rn_index];
	uint64_t result = (opc <= 1)? rn_val + imm12 : rn_val - imm12;
	if (sf == 0) {result = (uint32_t)result;}
	if ((opc & 1) == 0){ // without flag
		if (rd_index == REG32){
			return 0; // writes to ZR
		} 
		state->regs[rd_index] = result; // opc == 0 or 2
	} else { // with flag
		uint64_t rd_val = result; // opc == 1 or 3
		const int msb = (sf == 1)? 63 : 31;
		state->ps.N = (((rd_val >> msb) & 1) == 1); // set N if negative (MSB = 1)
		state->ps.Z = rd_val == 0; // set Z flag if = 0
		const bool rn_N = (rn_val >> msb) & 1 == 1; // check if rn_val is negative
		const bool imm12_N = (int32_t)imm12 < 0; // check if imm12 is negative
		if (opc == 1) {
			state->ps.C = rd_val < imm12;
			state->ps.V = (!state->ps.N && rn_N && imm12_N ) || (state->ps.N && !rn_N && !imm12_N );
		} else { 
			state->ps.C = rn_val >= imm12;
			state->ps.V = (state->ps.N && !rn_N && imm12_N ) || (!state->ps.N && rn_N && !imm12_N );
		}
		if (rd_index != REG32) { state->regs[rd_index] = rd_val; }
	}
	return 0;
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
			break;
		default:
			return 1;
	} 
	return 0;
}



