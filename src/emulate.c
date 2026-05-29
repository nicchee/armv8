#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>
#include <string.h>

static void init(mstate_t *mState_tp);
static void bin_file_load(uint8_t *memory, const char *fileName);
static void free_state(mstate_t *mState_tp);
static uint32_t fetch(mstate_t *mState_tp);
static void decode_execute(uint32_t ins, mstate_t *mState_tp);

// Instruction
#define SHIFT_OP0 25
#define MASK1110 14
#define MASK0111 7
#define MASK0101 5

#define DPI_IMM 8
#define DPI_REG 5
#define LOADSTORE 4
#define BRANCH 10

// End of Instruction Macros


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
static int dpi_imm_arith(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);
static int dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);

// Decode and execute stage - skeleton
static void decode_execute(uint32_t ins, mstate_t *mState_tp) {
	uint8_t op0 = ins >> SHIFT_OP0;

	if ((op0 & MASK1110) == DPI_IMM) { // checks 100x
		dpi_imm(ins, mState_tp);
	} else if ((op0 & MASK0111) == DPI_REG) { // checks x101 
		dpi_reg(ins, mState_tp);
	} else if ((op0 & MASK0101) == LOADSTORE) { // checks x1x0
		// load store
	} else if ((op0 & MASK1110) == BRANCH) { // checks 101x
		// branch remember to dec pc by 4 before jmp by offset     
	} else {
		exit (EXIT_FAILURE);
	}

}

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		exit(EXIT_FAILURE);
	} 
	mstate_t mState;
	init(&mState);
	bin_file_load(mState.memory, argv[1]);
	free_state(&mState); // move to after the cycle?
	bool run = true;
	while (run) {
		uint32_t ins = fetch(&mState);
		if (ins == STOP) {
			output(&mState);
			break;
		} else {
			decode_execute(ins, &mState);
		}
	}
	return EXIT_SUCCESS;
}

// init();
// 		initialise machine state;
static void init(mstate_t *mState_tp) {
	memset(mState_tp->regs, 0, sizeof(mState_tp->regs)); // zero all registers
	mState_tp->zr = 0;
	mState_tp->pc = 0;
	mState_tp->ps = (pstate_t){false, true, false, false}; //set Z flag only
  	mState_tp->memory = malloc(MEMSIZE);
	memset(mState_tp->memory, 0, MEMSIZE); //set all memory to 0
}

// bin_file_load(memory, argv[1]);
// load .bin file into memory
static void bin_file_load(uint8_t *memory, const char *fileName){
	FILE *in = fopen(fileName, "rb");
	if (in == NULL) {
		fprintf(stderr, "Can't open input binary file");
		exit(EXIT_FAILURE);
	}
	fseek(in, 0, SEEK_END);
	int fileSize = ftell(in);
	rewind(in);
	if (fileSize > MEMSIZE) {
		fprintf(stderr, "Bin file too big");
		fclose(in);
		exit(EXIT_FAILURE);
	}
	size_t bytesRead = fread(memory, sizeof(uint8_t), fileSize, in);
	fclose(in);
}

// Freeing the memory (avoiding memory leak)
static void free_state(mstate_t *mState_tp) {
	free(mState_tp->memory);
}

// Fetch stage
static uint32_t fetch(mstate_t *mState_tp) {
	uint32_t ins = *( (uint32_t *)(mState_tp->memory + mState_tp->pc) ); 
	mState_tp->pc += 4;
	return ins;
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
		exit(1); // failed
	}
}



/* return dpi_imm_arith(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);
 * 		helper function to perform arith functions based on opc if opi == ARITH
 * @return 0 if successful 1 if fail
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
			return 0; // writes to SP
		} 
		state->regs[rd_index] = result; // opc == 0 or 2
	} else { // with flag
		// opc == 1 or 3
		const int msb = (sf == 1)? 63 : 31;
		state->ps.N = ((result >> msb) & 1) == 1; // set N if negative (MSB = 1)
		state->ps.Z = result == 0; // set Z flag if = 0
		const bool rn_N = ((rn_val >> msb) & 1) == 1; // check if rn_val is negative
		const bool imm12_N = (int32_t)imm12 < 0; // check if imm12 is negative
		if (opc == 1) {
			state->ps.C = result < imm12;
			state->ps.V = (!state->ps.N && rn_N && imm12_N ) || (state->ps.N && !rn_N && !imm12_N );
		} else { 
			state->ps.C = rn_val >= imm12;
			state->ps.V = (state->ps.N && !rn_N && imm12_N ) || (!state->ps.N && rn_N && !imm12_N );
		}
		if (rd_index != REG32) { state->regs[rd_index] = result; }
	}
	return 0;
}

/* return dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index);
 * 		helper function to perform wide functions based on opc if opi == WMOVE
 * @return 0 if successful 1 if fail
*/
static int dpi_imm_wide(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index) {
	int hw = ((instruction >> SHIFT_HW) & MASK(2));
	uint64_t imm16 = (instruction >> SHIFT_IMM16) & MASK(16);
	if (sf == 0 && hw > 1) {
		exit(1);
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
			exit(1);
	} 
	return 0;
}



