#ifndef INS_PARSER_H
#define INS_PARSER_H

#define HALT_OPERAND "x0"

void parse(char *line, uint32_t *addr, instruction_t ins);


#endif