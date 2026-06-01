#include<string.h>

#include "tokeniser.h"

static TokenisedLine init_tokenised_line( void );

TokenisedLine tokenise(char *line) {
	TokenisedLine result = init_tokenised_line();
	/*
	// clean comments
	char *commentptr = strstr(line, "//");
	if (commentptr != NULL) {
		*commentptr = '\0';
	}
	*/

	// check for label
	char *colonptr = strchr(line, ':');
	if (colonptr != NULL) {
		*colonptr = '\0';
		result.label = strdup(line);
		// store in symbol table?
		return result;
	}

	// opcode
	char *saveptr;
	char *op = strtok_r(line, " ", &saveptr);
	if (op == NULL) {
		return result;
	}
	result.opcode = strdup(op);

	// operands
	char *operand = strtok_r(NULL, " [],", &saveptr);
	while (operand != NULL && result.num_operands < MAX_OPERANDS) {
		if (strlen(operand) > 0) { // check that string is not empty
			result.operands[result.num_operands] = strdup(operand);
			result.num_operands++;
		}
		operand = strtok_r(NULL, " [],", &saveptr);
	}

	return result;
}

static TokenisedLine init_tokenised_line( void ) {
	TokenisedLine result = { NULL, NULL, { NULL, NULL, NULL, NULL }, 0 };
	return result;
}
