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

