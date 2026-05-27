#include <stdlib.h>
#include "emulate.h"

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		return EXIT_FAILURE;
	} 
	init();
	bin_file_load(memory, argv[1]);

  return EXIT_SUCCESS;
}

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
