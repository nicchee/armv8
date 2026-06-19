#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MEMSIZE (2*1024*1024)
#define INSTR_SIZE 4 
#define STOP 0x8a000000

// Special Register
#define REG32 31

// Masking function - generate the mask bits required
#define MASK(x) ((1U << (x)) - 1)

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
	uint8_t *memory;
} mstate_t;

extern void check_addr(uint64_t addr, int num_bytes);
extern bool check_instr_addr(uint64_t addr);
extern void print(mstate_t *state, FILE *fp);
extern void init(mstate_t *mstate_p);
extern void free_state(mstate_t *mstate_p);
extern uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes);
extern void pc_inc(mstate_t *state);
extern int64_t sgx(int32_t value, int bit_no);

#endif