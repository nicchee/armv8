#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>
#include <string.h>

static void init(mstate_t *mState_tp);
static void bin_file_load(uint8_t *memory, const char *fileName);
static void free_state(mstate_t *mState_tp);

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		exit(EXIT_FAILURE);
	} 
	mstate_t mState;
	init(&mState);
	bin_file_load(mState.memory, argv[1]);
	free_state(&mState);

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
