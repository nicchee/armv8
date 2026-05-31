#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <stdint.h>

typedef struct{
    //  DP Imm
    union {
        uint32_t rd;
        uint32_t rt;
    };

    union {
        uint32_t rn;
        uint32_t xn;
    };
    
    uint32_t imm12;
    uint32_t sh;
    uint32_t opi;
    uint32_t opc;
    uint32_t sf_dp;

    // DP Reg
    uint32_t operand; // for DP Reg
    uint32_t rm;
    uint32_t opr;
    uint32_t M;

    // SINGLE DATA
    uint32_t offset;
    uint32_t L;
    uint32_t U;
    uint32_t sf_ndp;
    int32_t simm19;

    // BRANH 
    int32_t simm26;
    uint32_t cond; 

} instruction_t;

#endif