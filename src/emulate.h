#ifndef EMULATE_H
#define EMULATE_H
#define MEMSIZE (2*1024*1024)
#define STOP 0x8a000000

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool N;
	bool Z;
	bool C; 
	bool V;
} pstate_t;

typedef struct { 
	uint64_t regs[31];
	uint64_t zr;
	uint64_t pc;
	pstate_t ps;
} mstate_t;

#endif

int init(void) {
	struct pstate_t pState = {0,1,0,0}; //set Z flag only
	uint64_t genRegs[31] = {0} //zero all general registers
	struct mstate_t mState = {genRegs, 0, 0, pState}; //set machine state
	uint8_t memory[MEMSIZE] = {0}; //set all memory to 0
	return 0;
}

void bin_file_load(uint8_t memory, const char *fileName){
	FILE *in = fopen(fileName, "rb");
	if (in == NULL) {
		fprintf(stderr, "Can't open input binary file");
		exit(1);
	}
	
}
