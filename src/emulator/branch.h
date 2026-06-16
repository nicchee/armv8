#ifndef BRANCH_H
#define BRANCH_H
#include "machine_state.h"
#include "instruction.h"

extern void branch(instruction_t instruction, mstate_t *state);

#endif
