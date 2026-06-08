#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <stdint.h>

//Flags for Single Data Transfer Address Mode and Branch Type
typedef enum {UNSIGNED_OFFSET, PRE_POST_INDEX, REG_OFFSET, LIT_LOAD} sdt_addr_mode_t; 
typedef enum {UNCOND, REG, COND} branch_t;

// Struct for instruction_t
struct instruction_t {

    uint32_t op0;  // defines operation: DP_IMM, DP_REG, LOAD_STORE, BRANCH 
		   // 0101 if HALT

    // only one of these structs will exist in memory at a time
    union {
	//Halt
	struct {uint32_t halt_val} halt;

        // DP Imm
        struct {
            uint32_t sf, opc, opi;
	    uint8_t rd; 
	    union {
		struct {uint8_t rn
			uint32_t sh, imm12;} arith;
	    	struct {uint32_t hw, imm16;} wide;
	    };
        } dp_imm;

        // DP Reg
        struct {
            uint8_t rd, rn, rm; 
	    uint32_t sf, opc, M, opr;
	    union {
		struct {uint32_t N, shift, imm6;} arith_logic;
		struct {uint32_t x;
			uint8_t ra;} mul;
	    };
        } dp_reg;

	// Single Data Transfer
        struct {
            uint8_t rt; 
	    uint32_t sf;
	    sdt_addr_mode_t mode;

	    // Either Single Data Transfer or Load Literal
	    union {
		// Single Data Transfer: mode == 0 or 1 or 2
	    	struct {
		    uint8_t xn;
		    uint32_t L;
		    union {
		    	struct {uint32_t U, imm12;} unsigned_offset; // mode = 0
		    	struct {uint32_t I;
			    	int32_t simm9;} pre_post_index; // mode == 1
			uint8_t xm; // reg_offset: mode == 2
		    };
		} sdt;

		// Load Literal: mode == 3
		int32_t simm19; 
	    };
        } load_store;

        // Branches
        struct {
            branch_t type;
            union {
	    	int32_t simm26; // uncond: type == 0
		uint8_t xn;	// reg: type == 1
		struct {int32_t simm19; 
			uint32_t cond;} cond_t; // cond: type == 2
	    };
        } branch;
    };

} ;

typedef struct instruction_t *instruction_t;

#endif
