#ifndef INS_PARSER_H
#define INS_PARSER_H

#include <stdint.h>

#define HALT_OPERAND "x0"

extern void parse(char *line, uint32_t *addr, instruction_t ins, symtable table);

#endif
