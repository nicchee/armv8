#include <stdio.h>
#include "morsegen.h"

void init_gen(FILE *out) {
    fputs(out, "ldr w0, mem\n");
    fputs(out, "ldr w1, on\n");
    fputs(out, "str w1, [x0, #0x08]\n");
    fputs(out, "ldr w2, ldelay50ms\n"); // w2 -> delay 0.5 second
    fputs(out, "ldr w3, ldelay1s\n");   // w3 -> delay 1 second
    fputs(out, "ldr w4, ldelay3s\n");   // w4 -> delay 3 second
    fputs(out, "ldr w1, l4\n");
}


// delays 3 seconds
void space_gen(FILE *out) {
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "delay3s:\n");
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "add w10, w10, #1\n");
    fputs(out, "cmp w10, w4\n");
    fputs(out, "b.ne delay3s\n");
}

// delays 1 second
void break_gen(FILE *out) {
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "delay1s:\n");
    fputs(out, "eor w10, w10, w10\n");
    fputs(out, "add w10, w10, #1\n");
    fputs(out, "cmp w10, w3\n");
    fputs(out, "b.ne delay1s\n");
}

// halt function
void end_gen(FILE *out) {
    fputs(out, "halt:\nand x0, x0, x0\n");
}

// define all labels
void def_gen(FILE *out) {
    fputs(out, "mem:\n.int 0x3f200000\n"); // mem addr
    fputs(out, "on:\n.int 0x8\n"); // turn on
    fputs(out, "ldelay50ms:\n.int 0x400000\n"); //delay 0.5 sec
    fputs(out, "ldelay1s:\n.int 0x800000\n"); //delay 1 sec
    fputs(out, "ldelay3s:\n.int 0x1800000\n"); //delay 3 sec
    fputs(out, "off:\n.int 0x200000\n"); // turn off
}