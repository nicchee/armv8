#ifndef DPI_ASSEMBLER_H
#define DPI_ASSEMBLER_H
#include "instruction.h"

extern uint32_t dpi_imm_assembler(instruction_t insp);
extern uint32_t dpi_reg_assembler(instruction_t insp);

#endif