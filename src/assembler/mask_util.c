#include <stdint.h>

#include "mask_util.h"

//instruction_t instr;
//imm = mask_n_bits(instr->branch.cond_t.simm, bits)
//  Truncate a signed value to its low 'bits' bits, ready for embedding in an 
//  instruction field. The decoder will sign-extend back.
uint32_t mask_n_bits(int32_t value, unsigned bits) {
    return (uint32_t)value & ((1u << bits) - 1);
}
