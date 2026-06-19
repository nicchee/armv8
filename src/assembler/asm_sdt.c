#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "instruction.h"
#include "asm_sdt.h"
#include "mask_util.h"

//result = assemble_load_store(instr)  
//  assemble data for Single Data Transfer / Load Literal into its binary representation 
uint32_t assemble_load_store(instruction_t instr) {
    switch (instr->load_store.mode) {
        case LIT_LOAD: {
            uint32_t imm19 = mask_n_bits(instr->load_store.simm19, 19);
            return (instr->load_store.sf << 30)
                | (0x18 << 24)
                | (imm19 << 5)
                | instr->load_store.rt;
        }

        case UNSIGNED_OFFSET: {
            return (instr->load_store.sf << 30)
                | (0xB8 << 24)
                | (instr->load_store.sdt.unsigned_offset.U << 24)
                | (instr->load_store.sdt.L << 22)
                | (instr->load_store.sdt.unsigned_offset.imm12 << 10)
                | (instr->load_store.sdt.xn << 5)
                | instr->load_store.rt;
        }

        case PRE_POST_INDEX: {
            uint32_t imm9 = mask_n_bits(instr->load_store.sdt.pre_post_index.simm9, 9);
            return (instr->load_store.sf << 30)
                | (0xB8 << 24)
                | (instr->load_store.sdt.L << 22)
                | (imm9 << 12)
                | (instr->load_store.sdt.pre_post_index.I << 11)
                | (0x1 << 10)
                | (instr->load_store.sdt.xn << 5)
                | instr->load_store.rt;
        }

        case REG_OFFSET: {
            return (instr->load_store.sf << 30)
                | (0xB8 << 24)
                | (instr->load_store.sdt.L << 22)
                | (0x81A << 10)
                | (instr->load_store.sdt.xm << 16)
                | (instr->load_store.sdt.xn << 5)
                | instr->load_store.rt;
        }
    }
    fprintf(stderr, "ERROR(assemble_load_store): Load or store doesn't work");
    return EXIT_FAILURE;
}
