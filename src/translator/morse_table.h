#ifndef MORSE_TABLE_H
#define MORSE_TABLE_H

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

extern int alpha_calc(char alpha);
extern int digit_calc(char digit);

#endif
