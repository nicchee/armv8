#include <stdlib.h>
#include <stdio.h>
#include "instruction.h"

uint32_t assemble_branch(instruction_t *instr) {
    switch (instr->branch.type) {

        case UNCOND: {
            uint32_t imm26 = mask_n_bits(instr->branch.simm26, 26);
            return 0x14000000 | imm26;
        }
        
        case REG: {
            return 0xD61F0000 | (instr->branch.xn << 5);
        }

        case COND: {
            uint32_t imm19 = mask_n_bits(instr->branch.cond_t.simm19, 19);
            uint32_t cond = instr->branch.cond_t.cond & 0xF;
            return 0x54000000 | (imm19 << 5) | cond;
        }
    }
    fprintf(stderr, "ERROR(assemble_branch): branching doesn't work");
    return EXIT_FAILURE;
}

// helper

/* Truncate a signed value to its low 'bits' bits, ready for embedding
 * in an instruction field. The decoder will sign-extend back.
 */
extern uint32_t mask_n_bits(int32_t value, unsigned bits) {
    return (uint32_t)value & ((1u << bits) - 1);
}
