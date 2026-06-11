#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "instruction.h"
#include "asm-sdt.h"

/** @function: Assemble data for Single Data Transfer / Load Literal into its binary representation
 *  @param: instruction 
 */
uint32_t assemble_load_store(instruction_t *instr) {
    switch (instr->load_store.mode) {
        case LIT_LOAD: {
            uint32_t imm19 = instr->load_store.simm19 & 0x7FFFF;
            return (instr->load_store.sf << 30)
                | (0x18 << 24)
                | (imm19 << 5)
                | instr->load_store.rt;
        }

        case UNSIGNED_OFFSET: {
            return (instr->load_store.sf << 30)
                | (0xB8 << 31)
                | (instr->load_store.sdt.unsigned_offset.U << 24)
                | (instr->load_store.sdt.L << 22)
                | (instr->load_store.sdt.unsigned_offset.imm12 << 21)
                | (instr->load_store.sdt.xn << 9)
                | instr->load_store.rt;
        }

        case PRE_POST_INDEX: {
            return (instr->load_store.sf << 30)
                | (0xB8 << 31)
                | (instr->load_store.sdt.unsigned_offset.U << 24)
                | (instr->load_store.sdt.L << 22)
                | (instr->load_store.sdt.pre_post_index.simm9 << 20)
                | (instr->load_store.sdt.pre_post_index.I << 11)
                | (0x1 << 10)
                | (instr->load_store.sdt.xn << 9)
                | instr->load_store.rt;
        }

        case REG_OFFSET: {
            return (instr->load_store.sf << 30)
                | (0xB8 << 31)
                | (instr->load_store.sdt.unsigned_offset.U << 24)
                | (instr->load_store.sdt.L << 22)
                | (0x81A << 21)
                | (instr->load_store.sdt.xm << 20)
                | (instr->load_store.sdt.xn << 9)
                | instr->load_store.rt;
        }
    }
    fprintf(stderr, "ERROR(assemble_load_store): Load or store doesn't work");
    return EXIT_FAILURE;
}