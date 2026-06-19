#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <stdint.h>
#include <stdbool.h>

//Flags for Single Data Transfer Address Mode and Branch Type
typedef enum {UNSIGNED_OFFSET, PRE_POST_INDEX, REG_OFFSET, LIT_LOAD} sdt_mode_t; 
typedef enum {UNCOND, REG, COND} branch_type_t;

#define HALT_INS 0x8a000000

// OP0
#define HALT_OP0 0 			//0000
#define DPI_OP0 8 			//100x
#define DPR_OP0 5 			//x101
#define DPR_MUL_OP0 13          //1101
#define LOAD_STORE_OP0 4 	//x1x0
#define BRANCH_OP0 10 		//101x
#define BRANCH_REG_OP0 11       //1011
#define INTDIR_OP0 1
#define EMPTY_SKIP_OP0 -1

// OPC
#define ADD_OPC 0 	//00
#define ADDS_OPC 1	//01
#define SUB_OPC 2	//10
#define SUBS_OPC 3	//11

#define MOVN_OPC 0	//00 
#define MOVZ_OPC 2	//10 
#define MOVK_OPC 3	//11 

#define AND_OPC 0 	//00
#define BIC_OPC 0 	//00
#define ORR_OPC 1 	//00
#define ORN_OPC 1 	//00
#define EOR_OPC 2 	//00
#define EON_OPC 2 	//00
#define ANDS_OPC 3 	//00
#define BICS_OPC 3 	//00

#define MUL_OPC 0	//00 

// OPI
#define ARITH_OPI 2 //010
#define WIDE_OPI 5 	//101

// OPR
#define ARITH_OPR 0x8 	// 1xx0
#define LOGIC_OPR 0x0	// 0xxx
#define MUL_OPR 0x8		// 1000

// SHIFT 
#define LSL_SHIFT 0	// 00
#define LSR_SHIFT 1	// 01
#define ASR_SHIFT 2	// 10
#define ROR_SHIFT 3     // 11
#define RSR_SHIFT 3	// 11 (ONLY FOR LOGICAL)

// BRANCH COND
#define B_EQ 0X0 // 0000
#define B_NE 0X1 // 0001
#define B_GE 0Xa // 1010
#define B_LT 0Xb // 1011
#define B_GT 0Xc // 1100
#define B_LE 0Xd // 1101
#define B_AL 0Xe // 1110

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

// Data Processing Instruction (Register)
// Position of each instruction part
#define SHIFT_OPERAND 10
#define SHIFT_RM 16
#define SHIFT_OPR 21
#define SHIFT_M 28
#define SHIFT_NEGATED 21
#define SHIFT_SHIFT 22
#define SHIFT_RA 10
#define SHIFT_X 15

// Branch Instruction
// Position of each instruction part
#define SHIFT_SIMM26 0
#define SHIFT_XN 5
#define SHIFT_COND 0
#define SHIFT_SIMM19 5

// Type of Branches
// Mask
#define MASKUNCONDOP 0xfc000000
#define MASKREGOP    0xffff0000
#define MASKCONDOP   0xfc000000
// Bit code
#define UNCONDOP  0x14000000
#define REGOP     0xd61f0000
#define CONDOP    0x54000000

// Single Data Transfer
// Position of each instruction part
#define SHIFT_RT 0
#define SHIFT_XN 5
#define SHIFT_SIMM19 5
#define SHIFT_OFFSET 10
#define SHIFT_L 22
#define SHIFT_U 24
#define SHIFT_SDT_SF 30
#define SHIFT_OP 31
#define SHIFT_XM 16

// Single Data Transfer
// Width of each instruction part
#define WIDTH_RT 5
#define WIDTH_XN 5
#define WIDTH_SIMM19 19
#define WIDTH_OFFSET 12
#define WIDTH_L 1
#define WIDTH_U 1
#define WIDTH_SF 1
#define WIDTH_OP 1
#define WIDTH_XM 5

#define SHIFT_OP0 25
#define MASK1110 14
#define MASK0111 7
#define MASK0101 5

#define DPI_IMM 8
#define DPI_REG 5
#define LOADSTORE 4
#define BRANCH 10

// Struct for instruction_t
struct instruction_t {
    uint32_t op0; 	// defines operation: DP_IMM, DP_REG, LOAD_STORE, BRANCH 
    // only one of these structs will exist in memory at a time
    union {
		//Halt
		struct {
			uint32_t halt_val;
		} halt;
		
		//Int Directive
		struct {
			bool is_label;
			uint32_t int_val;
			char *label;
		} int_dir;

		// DP Imm
		struct {
			uint32_t sf, opc, opi;
			uint8_t rd; 
			union {
				struct {
					uint8_t rn;
					uint32_t sh, imm12;
				} arith;
				struct {
					uint32_t hw; 
					uint64_t imm16;
				} wide;
			};
		} dp_imm;

		// DP Reg
		struct {
			uint8_t rd, rn, rm; 
			uint32_t sf, opc, M, opr;
			union {
				struct {
					uint32_t N, shift, imm6;
				} arith_logic;
				struct {
					uint32_t x;
					uint8_t ra;
				} mul;
			};
		} dp_reg;
		
		// Single Data Transfer
		struct {
			uint8_t rt; 
			uint32_t sf;
			sdt_mode_t mode;
			// Either Single Data Transfer or Load Literal
			
			union {
				// Single Data Transfer: mode == 0 or 1 or 2
				struct {
					uint8_t xn;
					uint32_t L;
					union {
						struct {
							uint32_t U, imm12;
						} unsigned_offset; // mode = 0
						struct {
							uint32_t I;
							int32_t simm9;
						} pre_post_index; // mode == 1
						uint8_t xm; // reg_offset: mode == 2
					};
				} sdt;
				// Load Literal: mode == 3
				int32_t simm19; 
			};
		} load_store;

		// Branches
		struct {
			branch_type_t type;
			union {
				int32_t simm26; // uncond: type == 0
				uint8_t xn;	// reg: type == 1
				struct {
					int32_t simm19; 
					uint32_t cond;
				} cond_t; // cond: type == 2
			};
		} branch;
    };
};

typedef struct instruction_t *instruction_t;

extern instruction_t makeins(uint32_t ins);
extern void init_ins(void);
extern void free_ins(void);

#endif
