#include <string.h>
#include <stdlib.h>
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
	if (saveptr == NULL) { return result; }
	char *start = saveptr;
	int depth = 0;
	while (*start == ' ') { start++; }
	for (char *p = start;;p++) {
		if (result.num_operands >= MAX_OPERANDS) { break; }
		if (*p == '[') { depth++; }
		if (*p == ']') { depth--; }
		if (*p == ',' && depth == 0) {
			*p = '\0';
			if (strlen(start) > 0) {
				result.operands[result.num_operands] = strdup(start);
				result.num_operands++;
			}
			start = p + 1;
			while (*start == ' ') { start++; }
		} else if (*p == '\0') {
			if (strlen(start) > 0) {
				result.operands[result.num_operands] = strdup(start);
				result.num_operands++;
			}
			break;
		}
	}
	
	return result;
}


void free_tl(TokenisedLine *tl) {
	if (tl == NULL) return;
	free(tl->label);
	free (tl->opcode);
	for (int i = 0; i < tl->num_operands; i++) {
		free (tl->operands[i]);
		tl->operands[i] = NULL;
	}
	tl->label = NULL;
	tl->opcode = NULL;
}

static TokenisedLine init_tokenised_line( void ) {
	TokenisedLine result = { NULL, NULL, { NULL, NULL, NULL, NULL }, 0 };
	return result;
}

