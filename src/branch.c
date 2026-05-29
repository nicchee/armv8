#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "branch.h"
#include "emulate.h"

// Move this to mstate in the very near future
#define INSTR_SIZE 4 
#define MASK(x) ((1<<(x)) - 1)

// Type of Branches

// Mask
#define MASKUNCONDOP 0xfa000000
#define MASKREGOP    0x11111a1f
#define MASKCONDOP   0x1100001f

// Bit code
#define UNCONDOP  0x18000000 
#define REGOP     0xb61f0000
#define CONDOP    0x54000000

// Operand position
#define SHIFT_SIMM26 0
#define SHIFT_XN 5
#define SHIFT_COND 0
#define SHIFT_SIMM19 5

// Operand size
#define SIMM26_SIZE 26
#define REG_SIZE 5 // Move this to Mstate or something. everyone need this
#define SIMM19_SIZE 19
#define COND_SIZE 4

// Conditions
enum {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} Cond;

static int check_cond(int cond, pstate_t pstate);

// public 

void branch(uint32_t instruction, mstate_t *state){
    int64_t offset = 0;
    if ((instruction & MASKUNCONDOP) == UNCONDOP) {
        int32_t simm26 = (instruction >> SHIFT_SIMM26) & MASK(SIMM26_SIZE);
        state->pc += (int64_t)(simm26) * INSTR_SIZE; // PC = PC + simm26
    } else if ((instruction & MASKREGOP) == REGOP) {
        state->pc = state->regs[(instruction >> SHIFT_XN) & MASK(REG_SIZE)]; // PC = Xn
    } else if ((instruction & MASKCONDOP) == CONDOP) {
        int cond = (instruction >> SHIFT_SIMM26) & MASK(COND_SIZE);
        int32_t simm19 = (instruction >> SHIFT_SIMM19) & MASK(SIMM19_SIZE);
        int branch_cond = check_cond(cond, state->ps);
        switch (branch_cond) {
            case -1: 
                fprinf(stderr, "Error: Instruction not recognised for any branch operation.\n");
                exit(EXIT_FAILURE); // not recognised branch operation
            case 0: 
                return; // does not satisfies cond
            case 1: 
                state->pc += (int64_t)(simm19) * INSTR_SIZE; // PC = PC + simm19
                return; // satisfies cond
        }
    } else {
        fprinf(stderr, "Error: Instruction not recognised for any branch operation.\n");
        exit(EXIT_FAILURE);
    }
}

// helper 

/**
 * branch_cond = check_cond(cond, pstate);
 *  check if condition is true to branch
 *  @return 1 if true, 0 if false, -1 if cond not recognised
*/
static int check_cond(int cond, pstate_t pstate) { 
    switch (cond) {
        case EQ: return pstate.Z == 1;
        case NE: return pstate.Z == 0;
        case GE: return pstate.N == pstate.V; 
        case LT: return pstate.N != pstate.V;
        case GT: return pstate.Z == 0 && pstate.N == pstate.V;
        case LE: return !(pstate.Z == 0 && pstate.N == pstate.V);
        case AL: return 1;
        default: return -1;
        }
}