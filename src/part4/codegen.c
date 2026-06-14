#include "codegen.h"
#include "morse_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


static void dot_gen(FILE *out); 
static void dash_gen(FILE *out);

#define MORSELEN 10

typedef char morsecodeline[MORSELEN];

void codegen(FILE *out, char c) { 
    morsecodeline code;
    if (isalpha(c)) {
        strcpy(code, morse_alpha[alpha_calc(c)]);
    } else {
        strcpy(code, morse_alpha[digit_calc(c)]);
    }
    for(int i = 0; i < strlen(code); i++ ){
        if (code[i] == '.') {
            dot_gen(out);
        } else {
            dash_gen(out);
        }
    }
}

// turn on, delay 0.5 second, turn off
void dot_gen(FILE *out) {
    // turn on
    fputs("str w1, [x0, #0x1c]\n", out);

    // delay 0.5 second
    fputs("eor w10, w10, w10\n", out);
    fputs("delay50ms:\n", out);
    fputs("eor w10, w10, w10\n", out);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w2\n", out);
    fputs("b.ne delay50ms\n", out);

    // turn off
    fputs("str w1, [x0, #0x1c]\n", out);
}

// turn on, delay 1 second, turn off
void dash_gen(FILE *out) {
    // turn on
    fputs("str w1, [x0, #0x1c]\n", out);

    // delay 0.5 second
    fputs("eor w10, w10, w10\n", out);
    fputs("delay1s:\n", out);
    fputs("eor w10, w10, w10\n", out);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w3\n", out);
    fputs("b.ne delay1s\n", out);

    // turn off
    fputs("str w1, [x0, #0x1c]\n", out);
}