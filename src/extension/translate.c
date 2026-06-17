#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "codegen.h"
#include "morsegen.h"

//get 1-line input from stdin, call code_gen and space_gen 

#define MAXLINE 256
typedef char oneline[MAXLINE];

// Static helper prototype function
static void line_null_terminated(char *s);

int main(void) {
	printf("Usage: enter 1 line of input, alphabet/numbers/spaces only\n");
	printf("RPi will blink an LED in the morse code representation of your text\n");

	FILE *out = fopen("morse.s", "w");
	if (out == NULL) {
		fprintf(stderr, "Error opening output assembly file");
		exit(EXIT_FAILURE);
	}

	oneline line;
	fgets(line, MAXLINE, stdin);
	line_null_terminated(line);

	// initialising
	init_gen(out);

	// generating characters
    for (int i = 0; i < strlen(line); i++) {
    	if (isalnum(line[i])) {
        	codegen(out, line[i], i);
        	break_gen(out, i);
		} else if (isspace(line[i])) {
			space_gen(out, i);
		} else {
			fprintf(stderr, "Invalid input: can only contain alphabetical characters, numbers and spaces. No punctuation");
			exit(EXIT_FAILURE);
		}
	}

	// halt
	end_gen(out);

	// definition
	def_gen(out);

	fprintf(stdout, "Complete assembly code in morse.s\n");
	fclose(out);
}

// Helper 

//line_null_terminated(s);
//	check if s is null_terminated;
static void line_null_terminated(char *s) {
    for (int i = 0; i < MAXLINE; i++) {
        if (s[i] == '\0') {
            return;
        }
    }
    fprintf(stderr, "String is not null terminated!");
    exit(EXIT_FAILURE);
}
