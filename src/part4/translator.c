#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "codegen.h"
#include "morse_table.h"
#include "morsegen.h"


//get 1-line input from stdin, call code_gen and space_gen 

#define MAXLINE 256
typedef char oneline[MAXLINE];

static void line_null_terminated(char *s);

int main(int argc, char **argv) {
	printf("Usage: enter 1 line of input, alphabet/numbers/spaces only\n");
	printf("RPi will blink an LED in the morse code representation of your text\n");

	FILE *out = fopen("morse.s", "w");
	if (out == NULL) {
		fprintf(stderr, "Error opening output assembly file");
		exit(EXIT_FAILURE);
	}

	oneline line;
	fgets(line, MAXLINE, stdin);
    for (int i = 0; i < strlen(line); i++) {
    	if (isalnum(line[i])) {
        	code_gen(out);
        	break_gen(out);
		} else if (isspace(line[i])) {
			space_gen(out);
		} else {
		fprintf(stderr, "Invalid input: can only contain alphabetical characters, numbers and spaces. No punctuation");
		exit(EXIT_FAILURE);
		}
	}

    
	// FILE *in = fopen(argv[1], "r");
	// if (in == NULL) {
	// 	fprintf(stderr, "Error opening assembly file");
	// 	exit(EXIT_FAILURE);
	// }
    oneline s;  
    line_null_terminated(s);
    for (int i = 0; i < strlen(s); i++) {
        if(isalnum(s[i])) {
            codegen(s[i]);
        } else { 

        } 
    }
	fclose(out);
}

// morse_table[] {
//     97 -> ['a'] = &parse_a 
// }

// s <- input "abc"

// while loop 
//     a = morse_alpha[alpha_calc(s[0])] -> ".-"
//     a = ".-"
//     while (i = 0, )
//         codegen(a[i]) 

static void line_null_terminated(char *s) {
    for (int i = 0; i < MAXLINE; i++) {
        if (s[i] == '\0') {
            return;
        }
    }
    fprintf(stderr, "String is not null terminated!");
    exit(EXIT_FAILURE);
}
