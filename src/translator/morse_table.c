
#include "morse_table.h"
#include <ctype.h>

// index = alpha_calc()
int alpha_calc(char alpha) {
    return tolower(alpha) - 'a'; 
}

int digit_calc(char digit) { 
    return digit - '0';
}



