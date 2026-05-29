#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "emulate.h"
#include "dpi.h"
#include "single_data_transfer.h"


// Instruction
static void init(mstate_t *mstate_p);
static void bin_file_load(uint8_t *memory, const char *fileName);
static void free_state(mstate_t *mstate_p);
static uint32_t fetch(mstate_t *mstate_p);
static void decode_execute(uint32_t ins, mstate_t *mstate_p);

#define SHIFT_OP0 25
#define MASK1110 14
#define MASK0111 7
#define MASK0101 5

#define DPI_IMM 8
#define DPI_REG 5
#define LOADSTORE 4
#define BRANCH 10

// End of Instruction Macros

// Mask Macro
#define MASK(x) ((1<<(x)) - 1)

// Special Register
#define REG32 31

// Decode and execute stage - skeleton
static void decode_execute(uint32_t ins, mstate_t *mstate_p) {
	uint8_t op0 = ins >> SHIFT_OP0;

	if ((op0 & MASK1110) == DPI_IMM) { // checks 100x
		dpi_imm(ins, mstate_p);
	} else if ((op0 & MASK0111) == DPI_REG) { // checks x101 
		dpi_reg(ins, mstate_p);
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
	while (true) {
		uint32_t ins = fetch(&mState);
		if (ins == STOP) {
			//output(&mState);
			break;
		} else {
			decode_execute(ins, &mState);
		}
	}
	return EXIT_SUCCESS;
}

static void init(mstate_t *mstate_p) {
	memset(mstate_p->regs, 0, sizeof(mstate_p->regs)); // zero all registers
	mstate_p->zr = 0;
	mstate_p->pc = 0;
	mstate_p->ps = (pstate_t){false, true, false, false}; //set Z flag only
  	mstate_p->memory = malloc(MEMSIZE);
	memset(mstate_p->memory, 0, MEMSIZE); //set all memory to 0
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
static void free_state(mstate_t *mstate_p) {
	free(mstate_p->memory);
}

// Fetch stage
static uint32_t fetch(mstate_t *mstate_p) {
	if( mstate_p->pc + 4 > MEMSIZE || (int64_t)mstate_p->pc < 0 ) { // check if memory out of bound
		fprintf(stderr, "Memory address out of bound!\n");
		exit(EXIT_FAILURE);
	}

	if(mstate_p->pc % 4) { // check if memory address is 4-byte aligned
		fprintf(stderr, "Instruction Address is not aligned on a 4 byte boundary!\n");
		exit(EXIT_FAILURE);
	}

	uint8_t *instraddr = (mstate_p->memory + mstate_p->pc);

	uint32_t ins = 	(uint32_t) *instraddr
					| (uint32_t) *(instraddr+1) << 8
					| (uint32_t) *(instraddr+2) << 16
					| (uint32_t) *(instraddr+3) << 24;
	return ins;
}













