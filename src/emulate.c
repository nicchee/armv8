#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "emulate.h"

static uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes);
static void print(mstate_t *state, FILE *fp);

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

void print(mstate_t *state, FILE *fp) {
	fprintf(fp, "Registers:\n")
	for (int i = 0; i < 31, i++) {
		fprintf(fp, "X%02d = %016lx\n", i, state->regs[i]);
	}
	fprintf(fp, "\nPC = %lu\n", state->pc);
	fprintf(fp, "\nPSTATE : ");
	if (state->pc.N == true) {
		fprintf(fp, "N");
	} else {
		fprintf(fp, "-");
	}
	if (state->pc.Z == true) {
		fprintf(fp, "Z");
	} else {
		fprintf(fp, "-");
	}
	if (state->pc.C == true) {
		fprintf(fp, "C");
	} else {
		fprintf(fp, "-");
	}	
	if (state->pc.V == true) {
		fprintf(fp, "V\n");
	} else {
		fprintf(fp, "-\n");
	}
	fprintf(fp, "Non-zero memory:\n")
	for (uint64_t addr = 0; addr < MEMSIZE; addr += 4) {
		uint32_t result = read_memory(state, addr, 4);
		if (state.memory[i] != 0) {
			fprintf(fp, "0x%08lx: 0x%08x\n", addr, result);
		}
	}
}

static uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes) {
  check_addr(addr, num_bytes);
  uint64_t value = 0;
  for (int i = 0; i < num_bytes; i++) {
    value |=  ((uint64_t)state->memory[addr + i]) << (i * 8);
  }
  return value;
}

