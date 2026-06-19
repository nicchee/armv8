#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "branch.h"
#include "machine_state.h"
#include "instruction.h"

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

//  Static helper prototype function
static int check_cond(int cond, pstate_t pstate); 

void branch(instruction_t instruction, mstate_t *state){
    if (instruction->branch.type == UNCOND) {
        int32_t simm26 = instruction->branch.simm26;
	simm26 = (simm26 << 6) >> 6; // shift left by 6 to push bit 25 to bit 31, then asr by 6 to copy bit 31 to upper slots
        state->pc += (int64_t)(simm26) * INSTR_SIZE; // PC = PC + simm26
    } else if (instruction->branch.type == REG) {
        int xn = instruction->branch.xn;
        if (xn == REG32) { // encodes XZR
            pc_inc(state);
            return;
        } 
        state->pc = state->regs[xn]; // PC = Xn
    } else if (instruction->branch.type == COND) {
        int cond = instruction->branch.cond_t.cond;
        int32_t simm19 = instruction->branch.cond_t.simm19;
	simm19 = (simm19 << 13) >> 13; // shift left by 13 to push bit 18 to bit 31, then asr by 6 to copy bit 31 to upper slots
        int branch_cond = check_cond(cond, state->ps);
        switch (branch_cond) {
            case -1: 
                fprintf(stderr, "Error: Instruction not recognised for any branch operation.\n");
                exit(EXIT_FAILURE); // not recognised branch operation
            case 0:
		        pc_inc(state); 
                return; // does not satisfies cond
            case 1: 
                state->pc += (int64_t)(simm19) * INSTR_SIZE; // PC = PC + simm19
                return; // satisfies cond
        }
    } else {
        fprintf(stderr, "Error: Instruction not recognised for any branch operation.\n");
        exit(EXIT_FAILURE);
    }
}

// Helper 

//branch_cond = check_cond(cond, pstate);
//  check if condition is true to branch
//  return 1 if true, 0 if false, -1 if cond not recognised
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
