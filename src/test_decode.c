#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>
#include <stdint.h>

// Position of each instruction part for Single Data Transfer & Load Literal
#define SHIFT_RT 0
#define SHIFT_XN 5
#define SHIFT_SIMM19 5
#define SHIFT_OFFSET 10
#define SHIFT_L 22
#define SHIFT_U 24
#define SHIFT_SF 30
#define SHIFT_OP 31

// Width of each instrution part for Single Data Transfer & Load Literal
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

uint32_t instr[6] = {0xF9400260, 0xB9000C25, 0xF8408423, 0xF8606861, 0x58000040, 0xF85F8041};

int main(void) {
    for (int i = 0; i < 6; i++) {
        uint32_t instruction = instr[i];
        // Extraction of bits for Single Data Transfer & Load Literal
        uint32_t rt = (instruction >> SHIFT_RT) & MASK(WIDTH_RT);
        uint32_t xn = (instruction >> SHIFT_XN) & MASK(WIDTH_XN);
        uint32_t simm19 = (instruction >> SHIFT_SIMM19) & MASK(WIDTH_SIMM19);
        uint32_t offset = (instruction >> SHIFT_OFFSET) & MASK(WIDTH_OFFSET);
        uint32_t l = (instruction >> SHIFT_L) & MASK(WIDTH_L);
        uint32_t u = (instruction >> SHIFT_U) & MASK(WIDTH_U);
        uint32_t sf = (instruction >> SHIFT_SF) & MASK(WIDTH_SF);
        uint32_t op = (instruction >> SHIFT_OP) & MASK(WIDTH_OP);
        printf("Instruction %d (0x%08x):\n", i, instruction);
        printf(" op=%u, sf=%u, U=%u, L=%u\n", op, sf, u, l);
        printf(" rt=%u, xn=%u, simm19=%u, offset=%u\n", rt, xn, simm19, offset);
    }
    return 0;
}

