#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>

extern int init(mstate_t *mState_tp);
extern void bin_file_load(uint8_t memory, const char *fileName);

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		exit(EXIT_FAILURE);
	} 
	mstate_t mState;
	init(&mState);
	bin_file_load(memory, argv[1]);

  return EXIT_SUCCESS;
}

// init();
// 		initialise machine state;
int init(mstate_t *mState_tp) {
	memset(mState_tp->regs, 0, sizeof(mState_tp->regs)); // zero all registers
	mState_tp->zr = 0;
	mState_tp->pc = 0;
	mState_tp->ps = (pstate_t){false, true, false, false}; //set Z flag only
	uint8_t memory[MEMSIZE] = {0}; //set all memory to 0
	return 0;
}

// bin_file_load(memory, argv[1]);
//		load .bin file into memory
void bin_file_load(uint8_t memory, const char *fileName){
	FILE *in = fopen(fileName, "rb");
	if (in == NULL) {
		fprintf(stderr, "Can't open input binary file");
		exit(1);
	}
	
}
