#include <stdlib.h>
#include <string.h>
#include "asm-sdt.h"

/** @function: Assemble data for Single Data Transfer / Load Literal
 * @param: TokenisedLine (an array of strings), Program Counter (64-bit pc), A 
 */

uint32_t asm_sdt(TokenisedLine *tok, uint64_t pc, symtable_t *symtab) {
    // Parse Rt from Tok[0]
    int rt = parse_regs(tok[1]); // "x0" -> 0
    int sf = is_64_bit(tok[1]); // 64-bit = 1, 32-bit = 0
}


// Helper

int parse_regs(char *reg) {
    for (int i = 1; i < strlen(reg); i++) {
        reg[i-1] = reg[i];
    }
    reg[strlen(reg)] = "\0"
    return atoi(reg);
}

int is_64_bit(char* reg) {
    if (strcmp(reg[0], x, 1) == 0) { // w < x => negative value
        // 64-bit
        return 1;
    } else {
        // 32-bit
        return 0;
    }
}