#ifndef TOKENISER_H
#define TOKENISER_H

#define MAX_OPERANDS 4

typedef struct {
	char *label;
	char *opcode;
	char *operands[MAX_OPERANDS];
	int num_operands;
} TokenisedLine;

extern TokenisedLine tokenise(char *line);

#endif
