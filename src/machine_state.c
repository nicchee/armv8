#define MEMSIZE (2*1024*1024)

// Masking function - generate the mask bits required
#define MASK(x) ((1U << (x)) - 1)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_state.h"


void print(mstate_t *state, FILE *fp) {
	fprintf(fp, "Registers:\n");
	for (int i = 0; i < 31; i++) {
		fprintf(fp, "X%02d = %016lx\n", i, state->regs[i]);
	}
	fprintf(fp, "PC = %016lx\n", state->pc);
	fprintf(fp, "\nPSTATE : ");
	if (state->ps.N == true) {
		fprintf(fp, "N");
	} else {
		fprintf(fp, "-");
	}
	if (state->ps.Z == true) {
		fprintf(fp, "Z");
	} else {
		fprintf(fp, "-");
	}
	if (state->ps.C == true) {
		fprintf(fp, "C");
	} else {
		fprintf(fp, "-");
	}	
	if (state->ps.V == true) {
		fprintf(fp, "V\n");
	} else {
		fprintf(fp, "-\n");
	}
	fprintf(fp, "Non-zero memory:\n");
	for (uint64_t addr = 0; addr < MEMSIZE; addr += 4) {
		uint32_t result = read_memory(state, addr, 4);
		if (state->memory[addr] != 0) {
			fprintf(fp, "0x%08lx: %08x\n", addr, result);
		}
	}
	fprintf(fp, "0x%08lx : %08u\n", state->pc, STOP);
}

void init(mstate_t *mstate_p) {
	memset(mstate_p->regs, 0, sizeof(mstate_p->regs)); // zero all registers
	mstate_p->zr = 0;
	mstate_p->pc = 0;
	mstate_p->ps = (pstate_t){false, true, false, false}; //set Z flag only
  	mstate_p->memory = malloc(MEMSIZE);
	memset(mstate_p->memory, 0, MEMSIZE); //set all memory to 0
}

// Freeing the memory (avoiding memory leak)
void free_state(mstate_t *mstate_p) {
	free(mstate_p->memory);
}

/*
  * @function: check if address is Out-of-Bounds
  *	check if its 4 byte aligned
*/
void check_addr(uint64_t addr, int num_bytes) {
	if( addr + num_bytes > MEMSIZE || (int64_t)addr < 0 ) { // check if memory out of bound
		fprintf(stderr, "Memory address out of bound!\n");
		exit(EXIT_FAILURE);
	}
	if(addr % 4) { // check if memory address is 4-byte aligned
		fprintf(stderr, "Instruction Address is not aligned on a 4 byte boundary!\n");
		exit(EXIT_FAILURE);
	}
}

uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes) {
  check_addr(addr, num_bytes);
  uint64_t value = 0;
  for (int i = 0; i < num_bytes; i++) {
    value |=  ((uint64_t)state->memory[addr + i]) << (i * 8);
  }
  return value;
}

/**
 * 	pc_inc(state);
 * 		PC = PC + 4
 */
void pc_inc(mstate_t *state) {
	state->pc += INSTR_SIZE;
}
