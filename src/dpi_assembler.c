#include <stdlib.h>
#include <stdint.h>

#include "dpi_assembler.h"
#include "instruction.h"

#define MASK1110 14
#define MASK0111 7
#define DPI_IMM 8
#define DPI_REG 5

// Data Processing Instruction Immediate + Register
// Position of each instruction part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_OPC 29
#define SHIFT_SF 31

// Data Processing Instruction (Immediate)
// Position of each instruction part
#define SHIFT_IMM12 10
#define SHIFT_SH 22
#define SHIFT_IMM16 5
#define SHIFT_HW 21
#define SHIFT_OPI 23

// OPI instruction
#define ARITH 2 
#define WMOVE 5

// Data Processing Instruction (Register)
// Position of each instruction part
#define SHIFT_OPERAND 10
#define SHIFT_RM 16
#define SHIFT_OPR 21
#define SHIFT_M 28

// Arithmetic and Logical
#define ARITH_OPR_MASK 9
#define ARITH_OPR 8
#define ARITH_BL_M 0
#define SHIFT_NEGATED 21
#define SHIFT_SHIFT 22
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

// Multiply
#define OPR_MULT 8
#define SHIFT_RA 10
#define SHIFT_X 15

static uint32_t dpi_imm_assembler(instruction_t *insp);
static uint32_t dpi_reg_assembler(instruction_t *insp);

uint32_t dpi_assembler(instruction_t *insp) {
    uint32_t machine_code = 0;
    if ((insp->op0 & MASK1110) == DPI_IMM) { // checks 100x
		return dpi_imm_assembler(insp);
	} else if ((insp->op0 & MASK0111) == DPI_REG) { // checks x101 
		return dpi_reg_assembler(insp);
    } else {
        exit(EXIT_FAILURE);
    }    
}

static uint32_t dpi_imm_assembler(instruction_t *insp) {
    uint32_t machine_code = 0;
    machine_code |= insp->dp_imm.sf << SHIFT_SF;
    machine_code |= insp->dp_imm.opc << SHIFT_OPC;
    machine_code |= (insp->op0 & MASK1110) << SHIFT_OP0;
    machine_code |= insp->dp_imm.opi << SHIFT_OPI;
    if (insp->dp_imm.opi == ARITH) {
        machine_code |= insp->dp_imm.arith.sh << SHIFT_SH;
        machine_code |= insp->dp_imm.arith.imm12 << SHIFT_IMM12;
        machine_code |= insp->dp_imm.arith.rn << SHIFT_RN;
    } else if (insp->dp_imm.opi == WMOVE) {
        machine_code |= insp->dp_imm.wide.hw << SHIFT_HW;
        machine_code |= insp->dp_imm.wide.imm16 << SHIFT_IMM16;
    } else {
        exit(EXIT_FAILURE);
    }
    machine_code |= insp->dp_imm.rd;
    return machine_code;
}

static uint32_t dpi_reg_assembler(instruction_t *insp) {
    uint32_t machine_code = 0;
    machine_code |= insp->dp_reg.sf << SHIFT_SF;
    machine_code |= insp->dp_reg.opc << SHIFT_OPC;
    machine_code |= insp->op0 << SHIFT_OP0;
    machine_code |= insp->dp_reg.opr << SHIFT_OPR;
    machine_code |= insp->dp_reg.rn << SHIFT_RN;
    machine_code |= insp->dp_reg.rm << SHIFT_RM;
    if (insp->dp_reg.M == ARITH_BL_M) {
        machine_code |= insp->dp_reg.arith_logic.imm6 << SHIFT_OPERAND;
    } else {
        machine_code |= insp->dp_reg.mul.x << SHIFT_X;
        machine_code |= insp->dp_reg.mul.ra << SHIFT_RA;
    }
    machine_code |= insp->dp_reg.rd;
    return machine_code;
}