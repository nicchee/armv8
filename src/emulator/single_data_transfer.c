#include <stdlib.h>
#include <stdio.h>

#include "machine_state.h"
#include "single_data_transfer.h"
#include "instruction.h"

// Private helper functions
static int64_t sgx(int32_t value, int bit_no);
static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt);

/*
  * @function: implement the single data transfer and load literal
*/
void single_data_transfer(instruction_t instruction, mstate_t *state) {

  uint32_t rt = instruction->load_store.rt;
  uint32_t sf = instruction->load_store.sf;
  
  switch (instruction->load_store.mode) {

    case LIT_LOAD: {
      int32_t simm19 = instruction->load_store.simm19;
      uint64_t addr = state->pc + (sgx(simm19, 19) * 4);
      if (sf == 1) {
        state->regs[rt] = read_memory(state, addr, 8);
      } else {
        state->regs[rt] = read_memory(state, addr, 4);
      }
      break;
    }
    
    case UNSIGNED_OFFSET: {
      uint32_t xn = instruction->load_store.sdt.xn;
      uint32_t l = instruction->load_store.sdt.L;
      uint32_t imm12 = instruction->load_store.sdt.unsigned_offset.imm12;
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
      break;
    }

    case PRE_POST_INDEX: {
      uint32_t xn = instruction->load_store.sdt.xn;
      uint32_t l = instruction->load_store.sdt.L;
      uint32_t i = instruction->load_store.sdt.pre_post_index.I;
      int32_t simm9 = instruction->load_store.sdt.pre_post_index.simm9;
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
      break;
    }

    case REG_OFFSET: {
      uint32_t xn = instruction->load_store.sdt.xn;
      uint32_t l = instruction->load_store.sdt.L;
      uint32_t xm = instruction->load_store.sdt.xm;
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
      break;
    }
  }

  pc_inc(state);
  return;
}

// Helper functions
static int64_t sgx(int32_t value, int bit_no) {
  int64_t result = value & MASK(bit_no);
  
	if ((result >> (bit_no - 1)) == 1) {
    result |= ~(uint64_t)MASK(bit_no);
  }
  return result;
}

static void write_memory(mstate_t *state, uint64_t addr, int num_bytes, uint64_t rt) {
  check_addr(addr, num_bytes);
  if (addr >= 0x3f200000 && addr <= 0x3f2000c0) { // GPIO
    // part 3
    printf("GPIO Write to address 0x%08lx\n", addr);
    return; 
  }
  uint64_t value = state->regs[rt];
  for (int i = 0; i < num_bytes; i++) {
    state->memory[addr + i] = (uint8_t)(value >> (i * 8));
  }
  return;
}
