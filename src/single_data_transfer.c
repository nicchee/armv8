#include <stdlib.h>
#include <stdio.h>
#include "machine_state.h"
#include "single_data_transfer.h"

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

// Masking for simm9 and simm19
#define MASK_SIMM9 0x1FF
#define MASK_BIT_9 0x100
#define MASK_SIMM19 0x7FFFF
#define MASK_BIT_19 0x40000

// Private helper functions
static int64_t sgx_simm19(int32_t value);
static int64_t sgx_simm9(int32_t value);
static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt);

/*
  * @function: implement the single data transfer and load literal
*/
void single_data_transfer(uint32_t instruction, mstate_t *state) {
  // Extraction of bits for Single Data Transfer & Load Literal
  uint32_t rt = (instruction >> SHIFT_RT) & MASK(WIDTH_RT); 
  uint32_t xn = (instruction >> SHIFT_XN) & MASK(WIDTH_XN);
  int32_t simm19 = (instruction >> SHIFT_SIMM19) & MASK(WIDTH_SIMM19);
  uint32_t offset = (instruction >> SHIFT_OFFSET) & MASK(WIDTH_OFFSET);
  uint32_t l = (instruction >> SHIFT_L) & MASK(WIDTH_L);
  uint32_t u = (instruction >> SHIFT_U) & MASK(WIDTH_U);
  uint32_t sf = (instruction >> SHIFT_SF) & MASK(WIDTH_SF);
  uint32_t op = (instruction >> SHIFT_OP) & MASK(WIDTH_OP);

  if (op == 1) {
    // Compute the base address
    if (u == 1) {
      // Unsigned Immediate Offset
      uint32_t imm12 = offset;
      if (sf == 1) {
        uint64_t uoffset = (uint64_t) imm12 * 8;
        uint64_t addr = state->regs[xn] + uoffset;
        if (l == 1) {
          state->regs[rt] = read_memory(state, addr, 8);
        } else {
          write_memory(state, addr, 8, rt);
        }
      } else {
        uint32_t uoffset = imm12 * 4;
        uint64_t addr = state->regs[xn] + uoffset;
        if (l == 1) {
          state->regs[rt] = read_memory(state, addr, 4);
        } else {
          write_memory(state, addr, 4, rt);
        }
      }
    } else {
      // Determine which operation from bit 0 of offset
      uint32_t det_bit = offset & 1;
      uint32_t xm = (offset >> SHIFT_XM) & MASK(WIDTH_XM);
      if (det_bit == 0) {
        // Register Offset
        uint64_t addr = state->regs[xn] + state->regs[xm];
        if (sf == 1) {
          if (l == 1) {
            state->regs[rt] = read_memory(state, addr, 8);
          } else {
            write_memory(state, addr, 8, rt);
          }
        } else {
          if (l == 1) {
            state->regs[rt] = read_memory(state, addr, 4);
          } else {
            write_memory(state, addr, 4, rt);
          }
        }
      } else {
        int64_t simm9 = sgx_simm9((offset >> 2) & MASK(9));
        uint32_t i = (offset >> 1) & 1;
        if (i == 0) {
          // Post-Indexed
          uint64_t addr = state->regs[xn];
          if (sf == 1) {
            if (l == 1) {
                state->regs[rt] = read_memory(state, addr, 8);
              } else {
                write_memory(state, addr, 8, rt);
              }
          } else {
            if (l == 1) {
              state->regs[rt] = read_memory(state, addr, 4);
            } else {
              write_memory(state, addr, 4, rt);
            }
          }
          state->regs[xn] += simm9;
        } else {
          // Pre-Indexed
          uint64_t addr = state->regs[xn] + simm9;
          if (sf == 1) {
            if (l == 1) {
              state->regs[rt] = read_memory(state, addr, 8);
            } else {
              write_memory(state, addr, 8, rt);
            }
          } else {
            if (l == 1) {
              state->regs[rt] = read_memory(state, addr, 4);
            } else {
              write_memory(state, addr, 4, rt);
            }
          }
          state->regs[xn] += simm9;
        }
      }
    }
  } else {
      // Literal (op == 0) (L == 1)
      uint64_t addr = state->pc + (sgx_simm19(simm19) * 4);
      if (sf == 1) {
        state->regs[rt] = read_memory(state, addr, 8);
      } else {
        state->regs[rt] = read_memory(state, addr, 4);
      }
  }
  pc_inc(state);
  return;
}

// Helper functions
static int64_t sgx_simm19(int32_t value) {
  int64_t result = value & MASK_SIMM19;
  if (result & MASK_BIT_19) {
    result |= ~MASK_SIMM19;
  }
  return result;
}

static int64_t sgx_simm9(int32_t value) {
  int64_t result = value & MASK_SIMM9;
  if (result & MASK_BIT_9) {
    result |= ~MASK_SIMM9;
  }
  return result;
}

static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt) {
  check_addr(addr, num_bytes);
  uint64_t value = state->regs[rt];
  for (int i = 0; i < num_bytes; i++) {
    state->memory[addr + i] = (uint8_t)(value >> (i * 8));
  }
  return;
}