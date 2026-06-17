#include <stdlib.h>
#include <stdint.h>

#include "dpi_assembler.h"
#include "instruction.h"

uint32_t dpi_imm_assembler(instruction_t insp) {
    uint32_t machine_code = 0;
    machine_code |= insp->dp_imm.sf << SHIFT_SF;
    machine_code |= insp->dp_imm.opc << SHIFT_OPC;
    machine_code |= (insp->op0 & MASK1110) << SHIFT_OP0;
    machine_code |= insp->dp_imm.opi << SHIFT_OPI;
    if (insp->dp_imm.opi == ARITH_OPI) {
        machine_code |= insp->dp_imm.arith.sh << SHIFT_SH;
        machine_code |= insp->dp_imm.arith.imm12 << SHIFT_IMM12;
        machine_code |= insp->dp_imm.arith.rn << SHIFT_RN;
    } else if (insp->dp_imm.opi == WIDE_OPI) {
        machine_code |= insp->dp_imm.wide.hw << SHIFT_HW;
        machine_code |= insp->dp_imm.wide.imm16 << SHIFT_IMM16;
    } else {
        exit(EXIT_FAILURE);
    }
    machine_code |= insp->dp_imm.rd;
    return machine_code;
}

uint32_t dpi_reg_assembler(instruction_t insp) {
    uint32_t machine_code = 0;
    machine_code |= insp->dp_reg.sf << SHIFT_SF;
    machine_code |= insp->dp_reg.opc << SHIFT_OPC;
    machine_code |= insp->op0 << SHIFT_OP0;
    machine_code |= insp->dp_reg.opr << SHIFT_OPR;
    machine_code |= insp->dp_reg.rn << SHIFT_RN;
    machine_code |= insp->dp_reg.rm << SHIFT_RM;
    if (insp->dp_reg.M == ARITH_BL_M) {
        machine_code |= insp->dp_reg.arith_logic.shift << SHIFT_SHIFT;
        machine_code |= insp->dp_reg.arith_logic.N << SHIFT_NEGATED;
        machine_code |= insp->dp_reg.arith_logic.imm6 << SHIFT_OPERAND;
    } else {
        machine_code |= insp->dp_reg.mul.x << SHIFT_X;
        machine_code |= insp->dp_reg.mul.ra << SHIFT_RA;
    }
    machine_code |= insp->dp_reg.rd;
    return machine_code;
}
