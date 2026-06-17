#include <stdlib.h>
#include <stdio.h>

#include "instruction.h"
#include "mask_util.h"

//result = assemble_branch(ins);
//  returns 32-bit instruction for branch instruction
uint32_t assemble_branch(instruction_t instr) {
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

