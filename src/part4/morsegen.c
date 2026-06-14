#include <stdio.h>
#include "morsegen.h"

void init_gen(FILE *out) {
    fputs("ldr w0, mem\n", out);
    fputs("ldr w1, on\n", out);
    fputs("str w1, [x0, #0x08]\n", out);
    fputs("ldr w2, ldelay50ms\n", out); // w2 -> delay 0.5 second
    fputs("ldr w3, ldelay1s\n", out);   // w3 -> delay 1 second
    fputs("ldr w4, ldelay3s\n", out);   // w4 -> delay 3 second
    fputs("ldr w1, l4\n", out);
}


// delays 3 seconds
void space_gen(FILE *out, int j) {
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delayspace%d:\n", j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w4\n", out);
    fprintf(out, "b.ne delayspace%d\n", j);
}

// delays 1 second
void break_gen(FILE *out, int j) {
    fputs("eor w10, w10, w10\n", out);
    fprintf(out, "delaybreak%d:\n", j);
    fputs("add w10, w10, #1\n", out);
    fputs("cmp w10, w3\n", out);
    fprintf(out, "b.ne delaybreak%d\n", j);
}

// halt function
void end_gen(FILE *out) {
    fputs("halt:\nand x0, x0, x0\n", out);
}

// define all labels
void def_gen(FILE *out) {
    fputs("mem:\n.int 0x3f200000\n", out); // mem addr
    fputs("on:\n.int 0x8\n", out); // turn on
    fputs("ldelay50ms:\n.int 0x200000\n", out); //delay 0.5 sec
    fputs("ldelay1s:\n.int 0x800000\n", out); //delay 1 sec
    fputs("ldelay3s:\n.int 0x1800000\n", out); //delay 3 sec
    fputs("off:\n.int 0x200000\n", out); // turn off
    fputs("l4:\n.int 0x200000\n", out);
}