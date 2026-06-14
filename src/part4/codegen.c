#include "codegen.h"
#include "morsegen.h"
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

// turn on, delay 0.5 second, turn off
void dot_gen(FILE *out) {
    // turn on
    fputs(out, "str w1, [x0, #0x1c]\n");

    // delay 0.5 second
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "delay50ms:\n");
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "add w10, w10, #1\n");
    fputs(out, "cmp w10, w2\n");
    fputs(out, "b.ne delay30ms\n");

    // turn off
    fputs(out, "str w1, [x0, #0x1c]\n");
}

// turn on, delay 1 second, turn off
void dash_gen(FILE *out) {
    // turn on
    fputs(out, "str w1, [x0, #0x1c]\n");

    // delay 0.5 second
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "delay1s:\n");
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "add w10, w10, #1\n");
    fputs(out, "cmp w10, w3\n");
    fputs(out, "b.ne delay1s\n");

    // turn off
    fputs(out, "str w1, [x0, #0x1c]\n");
}