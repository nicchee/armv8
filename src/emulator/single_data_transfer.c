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
    }

    case PRE_POST_INDEX: {
      uint32_t xn = instruction->load_store.sdt.xn;
      uint32_t l = instruction->load_store.sdt.L;
      uint32_t i = instruction->load_store.sdt.pre_post_index.I;
      uint32_t simm9 = instruction->load_store.sdt.pre_post_index.simm9;
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
    // Debugging for part 3
    printf("GPIO Write to address 0x%08lx\n", addr);
    return; 
  }
  uint64_t value = state->regs[rt];
  for (int i = 0; i < num_bytes; i++) {
    state->memory[addr + i] = (uint8_t)(value >> (i * 8));
  }
  return;
}

// static int64_t sgx_simm19(int32_t value) {
//     int64_t result = value & MASK_SIMM19;
//     if (result & MASK_BIT_19) {
//     result |= ~MASK_SIMM19;
//   }
//   return result;
// }

// static int64_t sgx_simm9(int32_t value) {
//   int64_t result = value & MASK_SIMM9;
//   if (result & MASK_BIT_9) {
//       result |= ~MASK_SIMM9;
//     }
//     return result;
//   }
  

// if (op == 1) {
//   // Compute the base address
//   if (u == 1) {
  //     // Unsigned Immediate Offset
  //     uint32_t imm12 = offset;
  //     if (sf == 1) {
    //       uint64_t uoffset = (uint64_t) imm12 * 8;
    //       uint64_t addr = state->regs[xn] + uoffset;
//       if (l == 1) {
  //         state->regs[rt] = read_memory(state, addr, 8);
  //       } else {
    //         write_memory(state, addr, 8, rt);
    //       }
    //     } else {
      //       uint32_t uoffset = imm12 * 4;
      //       uint64_t addr = state->regs[xn] + uoffset;
      //       if (l == 1) {
//         state->regs[rt] = read_memory(state, addr, 4);
//       } else {
//         write_memory(state, addr, 4, rt);
//       }
//     }
//   } else {
//     // Determine which operation from bit 0 of offset
//     uint32_t det_bit = offset & 1;
//     uint32_t xm = (offset >> SHIFT_XM) & MASK(WIDTH_XM);
//     if (det_bit == 0) {
  //       // Register Offset
//       uint64_t addr = state->regs[xn] + state->regs[xm];
//       if (sf == 1) {
  //         if (l == 1) {
//           state->regs[rt] = read_memory(state, addr, 8);
//         } else {
  //           write_memory(state, addr, 8, rt);
  //         }
  //       } else {
    //         if (l == 1) {
      //           state->regs[rt] = read_memory(state, addr, 4);
      //         } else {
        //           write_memory(state, addr, 4, rt);
        //         }
        //       }
        //     } else {
          //       int64_t simm9 = sgx((offset >> 2) & MASK(9));
          //       uint32_t i = (offset >> 1) & 1;
          //       if (i == 0) {
            //         // Post-Indexed
            //         uint64_t addr = state->regs[xn];
            //         if (sf == 1) {
              //           if (l == 1) {
                //               state->regs[rt] = read_memory(state, addr, 8);
//             } else {
  //               write_memory(state, addr, 8, rt);
//             }
//         } else {
  //           if (l == 1) {
    //             state->regs[rt] = read_memory(state, addr, 4);
    //           } else {
//             write_memory(state, addr, 4, rt);
//           }
//         }
//         state->regs[xn] += simm9;
//       } else {
  //         // Pre-Indexed
  //         uint64_t addr = state->regs[xn] + simm9;
  //         if (sf == 1) {
    //           if (l == 1) {
      //             state->regs[rt] = read_memory(state, addr, 8);
      //           } else {
        //             write_memory(state, addr, 8, rt);
        //           }
        //         } else {
          //           if (l == 1) {
            //             state->regs[rt] = read_memory(state, addr, 4);
            //           } else {
              //             write_memory(state, addr, 4, rt);
              //           }
              //         }
              //         state->regs[xn] += simm9;
              //       }
              //     }
              //   }
              // } else {
                //     // Literal (op == 0) (L == 1)
                //     uint64_t addr = state->pc + (sgx(simm19) * 4);
                //     if (sf == 1) {
                  //       state->regs[rt] = read_memory(state, addr, 8);
                  //     } else {
                    //       state->regs[rt] = read_memory(state, addr, 4);
                    //     }
                    // }
