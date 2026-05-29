#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>
#include <string.h>

static void init(mstate_t *mState_tp);
static void bin_file_load(uint8_t *memory, const char *fileName);
static void free_state(mstate_t *mState_tp);
static void fetch(mstate_t *mState_tp);
static void decode_execute(uint32_t ins, mstate_t *mState_tp);

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		exit(EXIT_FAILURE);
	} 
	mstate_t mState;
	init(&mState);
	bin_file_load(mState.memory, argv[1]);
	free_state(&mState);
	bool run = true;
	while (run) {
		ins = fetch(&mState);
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
static uint32_t fetch(mState *mState_tp) {
	uint32_t ins = *( (uint32_t *)(mState_tp->memory + mState_tp->pc) ); 
	mState_tp->pc += 4;
	return ins;
}

// Decode and execute stage - skeleton
static void decode_execute(uint32_t ins, mState *mState_tp) {
	uint32_t op0 = (ins >> 25) & 0xF; //get bits 28-25
	
	if ((op0 & 0xE) == 0x8) { // 100x & 1110
		dpi_imm(ins, mState_tp);
	} 
	else if ((op0 & 0x7) == 0x5) { // x101 & 0111 
		dpi_reg(ins, mState_tp);
	} 
	else if ((op0 & 0xB) == 0x8) { // 1x00 & 1011
		//Load/store
	}
	else if ((op0 & 0xE) == 0xA) { // 101x & 1110
		//branch - remember to dec pc by 4 before jmp by offset	       
	}

}
