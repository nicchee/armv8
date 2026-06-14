#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>

static void dot_gen(FILE *out); 
static void dash_gen(FILE *out);


void codegen(FILE *out, char *s) { //s - space | a morse code of a character a .-
    for (int i = 0 ;)
        if (isspace(s[i])) {
            space_gen
        } else if(isalpha) {

        }
	if (c == '.')
        dot_gen 
}

void dot_gen(FILE *out) {
    //fprintf(out, "ldr w1, l4\n")
// ldr w1, l4
// str w1, [x0, #0x1c]

// eor w10, w10, w10
// delay1:
// add w10, w10, #1
// cmp w10, w2
// b.ne delay1
}

void dash_gen(FILE *out) {
    
}