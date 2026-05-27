#include <stdlib.h>
#include "emulate.h"

// Position of each instruction part for Single Data Transfer & Load Literal
#define SHIFT_RT 0
#define SHIFT_XN 5
#define SHIFT_SIMM19 5
#define SHIFT_OFFSET 10
#define SHIFT_L 22
#define SHIFT_U 24
#define SHIFT_SF 30
#define SHIFT_OP 31

// Width of each instcution part for Single Data Transfer & Load Literal
#define WIDTH_RT 5
#define WIDTH_XN 5
#define WIDTH_SIMM19 19
#define WIDTH_OFFSET 12
#define WIDTH_L 1
#define WIDTH_U 1
#define WIDTH_SF 1
#define WIDTH_OP 1

// Masking function - generate the mask bits required
#define MASK(x) ((1U << (x)) - 1)


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}

int single_data_transfer(uint32_t instruction, mstate_t *state, int sf, int opc, int rd_index) {
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
    // Unsigned Immediate Offset
    if (u == 1) {
      uint32_t imm12 = offset;
      if (sf == 1) {
        uint64_t uoffset = (uint64_t) imm12 * 8;
        uint64_t addr = state->regs[xn] + uoffset;
      } else {
        uint32_t uoffset = imm12 * 4;
        uint64_t addr = state->regs[xn] + uoffset;
      }
    } else {
      // Determine which operation from bit 10 of offset
      uint32_t det_bit = offset & 1; // QUESTION: Is it better to use the MASK?
      uint32_t xm = (offset >> 5) & MASK(5);
      if (det_bit == 0) {
        // Register Offset
        uint64_t addr = state->regs[xn] + state->regs[xm];
      } else {
        uint32_t i = (offset >> 1) & 1;
        if (i == 0) {
          // Post-Indexed
          uint64_t addr = state->regs[xn];
          state->regs[xn] += simm19;
        } else {
          // Pre-Indexed
          uint64_t addr = state->regs[xn] + state->regs[xm];
          state->regs[xn] = addr;
        }
      }
    }
  } else {
      // Literal
      uint64_t addr = state->pc + (simm19 * 4);
  }
}