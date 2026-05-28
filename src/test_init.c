#include <stdlib.h>
#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*
    TO COMPILE ONLY THIS FILE:
    1. Add guard on the main function of emulate.c
    e.g: #ifndef TESTING
         init(....)
         #endif
    2. Add declaration of init in emulate.h
    3. Run gcc -Wall -DTESTING -o test_init test_init.c emulate.c
    4. Run ./test_init
    5. (To remove): rm test_init
*/

int main(void) {
    mstate_t mState_tp;
    init(&mState_tp);
    printf("CHECKING THE STATE OF REGISTERS AND MEMORY: \n");
    printf(" regs[0]=%lu, regs[1]=%lu, regs[2]=%lu\n", mState_tp.regs[0], mState_tp.regs[1], mState_tp.regs[2]);
    for (int i = 0; i < 31; i++) {
        assert(mState_tp.regs[i] == 0);
    }
    printf(" zr=%lu, pc=%lu\n", mState_tp.zr, mState_tp.pc);
    assert(mState_tp.zr == 0);
    assert(mState_tp.pc == 0);
    printf(" N=%d, Z=%d, C=%d, V=%d\n", mState_tp.ps.N, mState_tp.ps.Z, mState_tp.ps.C, mState_tp.ps.V);
    assert(mState_tp.ps.N == false);
    assert(mState_tp.ps.Z == true);
    assert(mState_tp.ps.C == false);
    assert(mState_tp.ps.V == false);
    printf(" memory[0]=%u, memory[1]=%u, memory[2]=%u\n", mState_tp.memory[0], mState_tp.memory[1], mState_tp.memory[2]);
    for (int i = 0; i < 31; i++) {
        assert(mState_tp.memory[i] == 0);
    }
}