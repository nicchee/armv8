#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "machine_state.h"
#include "branch.h"
#include "dpi.h"
#include "single_data_transfer.h"
#include "instruction.h"

// Instruction
static void bin_file_load(uint8_t *memory, const char *fileName);
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


int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s\n", argv[0]);
		exit(EXIT_FAILURE);
	} 
	init_ins();
	mstate_t mState;
	init(&mState);
	bin_file_load(mState.memory, argv[1]);
	while (true) {
		uint32_t ins = fetch(&mState);
		if (ins == STOP) {
			if (argc == 2) {
				print(&mState, stdout);
			}
			if (argc == 3) {
				FILE *out = fopen(argv[2], "w");
				if ( out == NULL ) {
					fprintf( stderr, "Cant create .out file\n");
				}
				print(&mState, out);
				if (ferror(out)) {
					fprintf (stderr, "Error occured when writing to .out file\n");
				}
				fclose(out);
			}
			break;
		} else {
			decode_execute(ins, &mState);
		}
	}
	free_ins();
	free_state(&mState);
	return EXIT_SUCCESS;
}

// Decode and execute stage - skeleton
static void decode_execute(uint32_t ins, mstate_t *mstate_p) {
	instruction_t instruction = makeins(ins);

	if ((instruction->op0 & MASK1110) == DPI_IMM) { // checks 100x
		dpi_imm(instruction, mstate_p);
	} else if ((instruction->op0 & MASK0111) == DPI_REG) { // checks x101 
		dpi_reg(instruction, mstate_p);
	} else if ((instruction->op0 & MASK0101) == LOADSTORE) { // checks x1x0
		single_data_transfer(instruction, mstate_p);
	} else if ((instruction->op0 & MASK1110) == BRANCH) { // checks 101x
		branch(instruction, mstate_p);
	} else {
		exit (EXIT_FAILURE);
	}
}

// bin_file_load(memory, argv[1]);
// load .bin file into memory
static void bin_file_load(uint8_t *memory, const char *fileName){
	FILE *in = fopen(fileName, "rb");
	if (in == NULL) {
		fprintf(stderr, "Can't open input binary file\n");
		exit(EXIT_FAILURE);
	}
	fseek(in, 0, SEEK_END);
	int fileSize = ftell(in);
	rewind(in);
	if (fileSize > MEMSIZE) {
		fprintf(stderr, "Bin file too big\n");
		fclose(in);
		exit(EXIT_FAILURE);
	}
	fread(memory, sizeof(uint8_t), fileSize, in);
	fclose(in);
}


// Fetch stage
static uint32_t fetch(mstate_t *mstate_p) {
	if (check_instr_addr(mstate_p->pc) == false){
		return STOP;
	}

	uint8_t *instraddr = (mstate_p->memory + mstate_p->pc);

	uint32_t ins = 	(uint32_t) *instraddr
					| (uint32_t) *(instraddr+1) << 8
					| (uint32_t) *(instraddr+2) << 16
					| (uint32_t) *(instraddr+3) << 24;
	return ins;
}
