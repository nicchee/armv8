#ifndef MORSE_TABLE_H
#define MORSE_TABLE_H

#include "morse_table.h"
#include <ctype.h>

// morse is an array of strings
const char *morse_alpha[] = {
    //alpha a to z
    ".-", // A
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.."
};

const char *morse_digit[] = {
    //digit 0 to 9 
    "-----", // 0
    ".---", 
    "..---",
    "...--",
    "....-",
    ".....",
    "-....",
    "--...",
    "---..",
    "----."
};


// index = alpha_calc()
int alpha_calc(char alpha) {
    return tolower(alpha) - 'a'; 
}

int digit_calc(char digit) { 
    return digit - '0';
}

#endif

