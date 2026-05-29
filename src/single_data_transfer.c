#include <stdlib.h>
#include <stdio.h>
#include "emulate.h"
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

// Private helper functions
static uint64_t sgx_simm19(uint32_t value);
static uint64_t sgx_simm9(uint32_t value);
static uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes);
static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt);



/*
  * @function: implement the single data transfer and load literal
*/
void single_data_transfer(uint32_t instruction, mstate_t *state) {
  // Extraction of bits for Single Data Transfer & Load Literal
  uint32_t rt = (instruction >> SHIFT_RT) & MASK(WIDTH_RT); 
  uint32_t xn = (instruction >> SHIFT_XN) & MASK(WIDTH_XN);
  uint32_t simm19 = (instruction >> SHIFT_SIMM19) & MASK(WIDTH_SIMM19);
  uint32_t offset = (instruction >> SHIFT_OFFSET) & MASK(WIDTH_OFFSET);
  uint32_t l = (instruction >> SHIFT_L) & MASK(WIDTH_L);
  uint32_t u = (instruction >> SHIFT_U) & MASK(WIDTH_U);
  uint32_t sf = (instruction >> SHIFT_SF) & MASK(WIDTH_SF);
  uint32_t op = (instruction >> SHIFT_OP) & MASK(WIDTH_OP);

  if (op == 0) {
    // Compute the base address
    if (u == 1) {
      // Unsigned Immediate Offset
      uint32_t imm12 = offset;
      if (sf == 1) {
        uint64_t uoffset = (uint64_t) imm12 * 8;
        uint64_t addr = state->regs[xn] + uoffset;
        if (l == 1) {
          read_memory(state, addr, 8);
        } else {
          write_memory(state, addr, 8, rt);
        }
      } else {
        uint32_t uoffset = imm12 * 4;
        uint64_t addr = state->regs[xn] + uoffset;
        if (l == 1) {
          read_memory(state, addr, 4);
        } else {
          write_memory(state, addr, 4, rt);
        }
      }
    } else {
      // Determine which operation from bit 0 of offset
      uint32_t det_bit = offset & 1; // QUESTION: Is it better to use the MASK?
      uint32_t xm = (offset >> SHIFT_XM) & MASK(WIDTH_XM);
      if (det_bit == 0) {
        // Register Offset
        uint64_t addr = state->regs[xn] + state->regs[xm];
        if (sf == 1) {
          if (l == 1) {
            read_memory(state, addr, 8);
          } else {
            write_memory(state, addr, 8, rt);
          }
        } else {
          if (l == 1) {
            read_memory(state, addr, 4);
          } else {
            write_memory(state, addr, 4, rt);
          }
        }
      } else {
        uint64_t simm9 = sgx_simm9((offset >> 2) & MASK(9));
        uint32_t i = (offset >> 1) & 1;
        if (i == 0) {
          // Post-Indexed
          uint64_t addr = state->regs[xn];
          if (sf == 1) {
          if (l == 1) {
              read_memory(state, addr, 8);
            } else {
              write_memory(state, addr, 8, rt);
            }
          } else {
            if (l == 1) {
              read_memory(state, addr, 4);
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
              read_memory(state, addr, 8);
            } else {
              write_memory(state, addr, 8, rt);
            }
          } else {
            if (l == 1) {
              read_memory(state, addr, 4);
            } else {
              write_memory(state, addr, 4, rt);
            }
          }
          state->regs[xn] += simm9;
        }
      }
    }
  } else {
      // Literal (op == 1) (L == 1)
      uint64_t addr = state->pc + (sgx_simm19(simm19) * 4);
      if (sf == 1) {
        read_memory(state, addr, 8);
      } else {
        read_memory(state, addr, 4);
      }
  }
  return;
}

// Helper functions
static uint64_t sgx_simm19(uint32_t value) {
  uint64_t result = value & 0x000000000007FFFF;
  if (result & 0x0000000000040000) {
    result |= ~0x000000000007FFFF;
  }
  return result;
}

static uint64_t sgx_simm9(uint32_t value) {
  uint64_t result = value & 0x00000000000003FF;
  if (result & 0x0000000000000200) {
    result |= ~0x00000000000003FF;
  }
  return result;
}

static uint64_t read_memory(mstate_t *state, uint64_t addr, int num_bytes) {
  check_addr(addr, num_bytes);
  uint64_t value = 0;
  for (int i = 0; i < num_bytes; i++) {
    value |=  ((uint64_t)state->memory[addr + i]) << (i * 8);
  }
  return value;
}

static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt) {
  check_addr(addr, num_bytes);
  uint64_t value = state->regs[rt];
  for (int i = 0; i < num_bytes; i++) {
    state->memory[addr + i] = (uint8_t)(value >> (i * 8));
  }
  return;
}


