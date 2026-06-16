#ifndef DPI_H
#define DPI_H
#include "instruction.h"

extern void dpi_imm(instruction_t instruction, mstate_t *state);
extern void dpi_reg(instruction_t instruction, mstate_t *state);

#endif

