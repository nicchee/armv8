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
#define LOAD_STORE_OP0 4 	//x1x0
#define BRANCH_OP0 10 		//101x
#define INTDIR_OP0 1

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
#define RSR_SHIFT 3	// 11 (ONLY FOR LOGICAL)

// BRANCH COND
#define B_EQ 0X0 // 0000
#define B_NE 0X1 // 0001
#define B_GE 0Xa // 1010
#define B_LT 0Xb // 1011
#define B_GT 0Xc // 1100
#define B_LE 0Xd // 1101
#define B_AL 0Xe // 1110


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
					uint32_t hw, imm16;
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

#endif
