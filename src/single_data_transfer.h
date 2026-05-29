#ifndef SINGLE_DATA_TRANSFER_H
#define SINGLE_DATA_TRANSFER_H

// Position of each instruction part for Single Data Transfer & Load Literal
#define SHIFT_RT 0
#define SHIFT_XN 5
#define SHIFT_SIMM19 5
#define SHIFT_OFFSET 10
#define SHIFT_L 22
#define SHIFT_U 24
#define SHIFT_SF 30
#define SHIFT_OP 31
#define SHIFT_XM 5

// Width of each instcution part for Single Data Transfer & Load Literal
#define WIDTH_RT 5
#define WIDTH_XN 5
#define WIDTH_SIMM19 19
#define WIDTH_OFFSET 12
#define WIDTH_L 1
#define WIDTH_U 1
#define WIDTH_SF 1
#define WIDTH_OP 1
#define WIDTH_XM 5

// Masking function - generate the mask bits required
#define MASK(x) ((1U << (x)) - 1)

// Prototypes for helper functions
static uint64_t sgx_simm19(uint32_t value);
static uint64_t sgx_simm9(uint32_t value);
static uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes);
static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt);
static void check_addr(uint64_t addr, int num_bytes);

#endif