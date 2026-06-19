#include <ctype.h>

#include "morse_table.h"

//  global array of morse code for each alpha
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

//  global array of morse code for each digit
const char *morse_digit[] = {
    //digit 0 to 9 
    "-----", // 0
    ".----", 
    "..---",
    "...--",
    "....-",
    ".....",
    "-....",
    "--...",
    "---..",
    "----."
};


//index = alpha_calc(a);
//  calculates index for character a.
//  starting with 'a' as 0
int alpha_calc(char alpha) {
    return tolower(alpha) - 'a'; 
}

//index = digit_calc(d);
//  calculates index for digit d
//  starting with '0' as 0;
int digit_calc(char digit) { 
    return digit - '0';
}


