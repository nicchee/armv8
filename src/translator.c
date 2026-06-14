#include <stdlib.h>
#include <stdio.h>

#include "codegen.h"
#include "morse_table.h"

#define MAX_LINE_LEN 64 

//get 1-line input from stdin, call code_gen and space_gen 

int main(int argc, char **argv) {
	printf("Usage: enter 1 line of input, alphabet/numbers/spaces only\n");
	printf("RPi will blink an LED in the morse code representation of your text\n");

    	FILE *out = fopen("morse.s", "w");
	if (out == NULL) {
		fprintf(stderr, "Error opening output assembly file");
		exit(EXIT_FAILURE);
	}

	char line[MAX_LINE_LEN];

	fgets(line, MAX_LINE_LEN, stdin);

    	for (int i = 0; i < strlen(line); i++) {
    		if (isalnum(line[i])) {
        		code_gen(out);
        		break_gen(out);
		}
		else if (isspace(line[i])) space_gen(out);

		else {
			fprintf(stderr, "Invalid input: can only contain alphabetical characters, numbers and spaces. No punctuation");
			exit(EXIT_FAILURE);
		}
	}

        fclose(out);
}

morse_table[] {
    97 -> ['a'] = &parse_a 
}

s <- input "abc"

while loop 
    a = morse_alpha[alpha_calc(s[0])] -> ".-"
    a = ".-"
    while (i = 0, )
        codegen(a[i]) 
