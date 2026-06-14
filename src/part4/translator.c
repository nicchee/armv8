#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "morsegen.h"
#include "codegen.h"

#define MAXLINE 256
typedef char oneline[MAXLINE];

static void line_null_terminated(char *s);

int main(int argc, char **argv) {
    // if (argc != 3) {
	// 	fprintf(stderr, "Usage: %s <assembly_file_in> <binary_file_out>", argv[0]);
	// 	exit(EXIT_FAILURE);
	// }

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
            break
        } else { 

        } else {
            
        }
    }

	

	// fclose(in);
	// return EXIT_SUCCESS;

    for 
    if digit 
    if alpha
    
        s = "hello world"
        fopen (out)
        if (!isspace(s[0])) {
        convert s[0] -> . -
        code_gen(out)
        break_gen(out)
        } else {
        space_gen(out)
        }
        fclode(out)
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

static void line_null_terminated(char *s) {
    for (int i = 0; i < MAXLINE; i++) {
        if (s[i] == '\0') {
            return;
        }
    }
    fprintf(stderr, "String is not null terminated!");
    exit(EXIT_FAILURE);
}