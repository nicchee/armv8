#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <stdint.h>

typedef struct {

    uint32_t op0;  // defines operation: DP_IMM, DP_REG, LOAD_STORE, BRANCH

    // only one of these structs will exist in memory at a time
    union {
        // DP Imm
        struct {
            uint32_t rd;
            uint32_t rn;
            uint32_t imm12;
            uint32_t sh;
            uint32_t sf;
            uint32_t opi;
        } dp_imm;

        // DP Reg
        struct {
            uint32_t rd;
            uint32_t rn;
            uint32_t rm;
            uint32_t operand;
            uint32_t sf;
            uint32_t opc;
            uint32_t M;
        } dp_reg;

        // SINGLE DATA TRANSFER
        struct {
            uint32_t rt;   
            uint32_t rn;
            uint32_t offset;
            uint32_t L;
            uint32_t sf;
        } load_store;

        // Category 4: Branches
        struct {
            int32_t  simm26;
            int32_t  simm19;
            uint32_t cond;
        } branch;
    };

} instruction_t;

#endif