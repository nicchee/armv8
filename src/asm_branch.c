#include <stdlib.h>
#include <stdio.h>
#include "instruction.h"

static uint32_t reverse_sgx(int32_t value, unsigned int bits);

uint32_t assemble_branch(instruction_t *instr) {
    switch (instr->branch.type) {

        case UNCOND: {
            uint32_t imm26 = reverse_sgx(instr->branch.simm26, 26);
            return (0x14 << 31)
                | (imm26 << 25);
        }
        
        case REG: {
            return (0xD61F << 31)
                | (instr->branch.xn << 9);
        }

        case COND: {
            uint32_t imm19 = reverse_sgx(instr->branch.cond_t.simm19, 19);
            uint32_t cond = instr->branch.cond_t.cond & 0xF;
            return (0x54 << 31)
                | (imm19 << 23)
                | cond;
        }
    }
fprintf(stderr, "ERROR(assemble_branch): branching doesn't work");
return EXIT_FAILURE;
}

// helper

/* Truncate a signed value to its low 'bits' bits, ready for embedding
 * in an instruction field. The decoder will sign-extend back.
 */
extern uint32_t reverse_sgx(int32_t value, unsigned bits) {
    return (uint32_t)value & ((1 << bits) - 1);
}
