#ifndef TOKENISER_H
#define TOKENISER_H

#define MAX_OPERANDS 4

typedef struct {
	char *label; // eg: L1: , L2: 
	char *opcode; // eg: add, sub 
	char *operands[MAX_OPERANDS]; // eg: [x0, x1]
	int num_operands; // eg: 2
} TokenisedLine;

extern TokenisedLine tokenise(char *line);

#endif 
