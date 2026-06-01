#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <stdint.h>

//Flags for Single Data Transfer Address Mode and Branch Type
typedef enum {UNSIGNED_OFFSET, PRE_POST_INDEX, REG_OFFSET, LIT_LOAD} sdt_address_mode_t; 
typedef enum {UNCOND, REG, COND} branch_type_t;

// Struct for instruction_t
typedef struct {

    uint32_t op0;  // defines operation: DP_IMM, DP_REG, LOAD_STORE, BRANCH

    // only one of these structs will exist in memory at a time
    union {
        // DP Imm
        struct {
            uint32_t sf, opc, opi;
	    uint64_t rd; // rd and rn are 64bit if sf == 1
	    union {
		struct {uint64_t rn
			uint32_t sh, imm12;} arith;
	    	struct {uint32_t hw, imm16;} wide;
	    };
        } dp_imm;

        // DP Reg
        struct {
            uint64_t rd, rn, rm; // 64bit if sf == 1
	    uint32_t sf, opc, M, opr;
	    union {
		struct {uint32_t N, shift, imm6;} arith_logic;
		struct {uint32_t x, ra;} mul;
	    };
        } dp_reg;

	// Single Data Transfer
        struct {
            uint64_t rt; //64bit if sf == 1
	    uint32_t sf;
	    sdt_addr_mode_t mode;

	    // Either Single Data Transfer or Load Literal
	    union {
		// Single Data Transfer: mode == 0 or 1 or 2
	    	struct {
		    uint64_t xn;
		    uint32_t L;
		    union {
		    	struct {uint32_t U, imm12;} unsigned_offset; // mode = 0
		    	struct {uint32_t I;
			    	int32_t simm9;} pre_post_index; // mode == 1
			uint64_t xm; // reg_offset: mode == 2
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
		uint64_t xn;	// reg: type == 1
		struct {int32_t simm19; 
			uint32_t cond;} cond_t; // cond: type == 2
	    };
        } branch;
    };

} instruction_t;

#endif
