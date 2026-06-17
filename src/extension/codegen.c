#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "codegen.h"
#include "morse_table.h"

// Static helper prototype function
static void dot_gen(FILE *out, int j, int i); 
static void dash_gen(FILE *out, int j, int i);

#define MORSELEN 10

typedef char morsecodeline[MORSELEN];

//codegen(out, c, j);
//  generate morse code to light up led based on c being dot or dashes. 
//  use j to create unique delay label
void codegen(FILE *out, char c, int j) { 
    morsecodeline code;
    if (isalpha(c)) {
        strcpy(code, morse_alpha[alpha_calc(c)]);
    } else {
        strcpy(code, morse_digit[digit_calc(c)]);
    }
    for(int i = 0; i < strlen(code); i++ ){
        if (code[i] == '.') {
            dot_gen(out, j, i);
        } else {
            dash_gen(out, j, i);
        }
    }
}

//dot_gen(out, j, i);
//  generate assembly code to turn on led, delay 0.25 second, turn off
void dot_gen(FILE *out, int j, int i) {
    // turn on
    fputs("str w1, [x0, #0x1c]\n", out);

    // delay 0.25 second
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delayon%d%d:\n",i ,j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w2\n", out);
    fprintf(out, "b.ne delayon%d%d\n",i ,j);

    // turn off
    fputs("str w1, [x0, #0x28]\n", out);

    // delay 0.25 second
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delayoff%d%d:\n",i ,j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w2\n", out);
    fprintf(out, "b.ne delayoff%d%d\n",i ,j);
}
// dash_gen(out, j, i);
//  generate assembly code to turn onled , delay 1 second, turn off
void dash_gen(FILE *out, int j, int i) {
    // turn on
    fputs("str w1, [x0, #0x1c]\n", out);

    // delay 1 second
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delay%d%d:\n",i ,j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w3\n", out);
    fprintf(out, "b.ne delay%d%d\n",i ,j);

    // turn off
    fputs("str w1, [x0, #0x28]\n", out);

    // delay 0.25 second
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delayoff%d%d:\n",i ,j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w2\n", out);
    fprintf(out, "b.ne delayoff%d%d\n",i ,j);
}
