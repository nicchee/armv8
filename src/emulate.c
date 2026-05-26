#include <stdlib.h>
#include "emulate.h"

// Position of each instrution part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_IMM12 10
#define SHIFT_SH 22
#define SHIFT_IMM16 5
#define SHIFT_HW 21
#define SHIFT_OPI 23
#define SHIFT_OPC 29
#define SHIFT_SF 31
#define MASK(x) ((1<<x) - 1)

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
	if (opi == 2) {
		
	} else if (opi == 5) {
	
	} else {
		return 1;
	}
}

int dpi_imm_arith(uint32_t instruction, mstate_t *state) {
	uint32_t imm12 = (instruction >> SHIFT_IMM12) & MASK(12);
	int sh = (instruction >> SHIFT_SF) & MASK(1);
	if (sh == 1) {
		imm12 <<= 12 	
	}
	
	
	
}



