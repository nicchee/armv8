#include "morse_table.h"
#include <ctype.h>

int alpha_calc(char alpha) {
    return tolower(alpha) - 'a';
}

int digit_calc(char digit) {
    return digit - 1;
}