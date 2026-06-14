#ifndef MORSE_TABLE_H
#define MORSE_TABLE_H

// morse is an array to string
const char *morse_alpha[] = {
    //alpha a to z
    ".-\0", // A
    // B 
    // C 
    // STUB
};



const char *morse_digit[] = {
    //digit 0 to 9 ORDER IS IMPORTANT 
    ".---\0", // 1
    // STUB

};


extern int alpha_calc(char alpha);
extern int digit_calc(char digit);

#endif
