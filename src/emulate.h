#ifndef EMULATE_H
#define EMULATE_H

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
	uint64_t pc;
	pstate_t ps;
	uint8_t *memory
} mstate_t;

#define MEMSIZE  2 * 1024 * 1024

#endif
