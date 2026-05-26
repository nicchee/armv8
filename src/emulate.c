#include <stdlib.h>
#include "emulate.h"

// Data Processing Instruction (Register)

// Position of each instruction part
#define SHIFT_RD 0
#define SHIFT_RN 5
#define SHIFT_OPERAND 10
#define SHIFT_RM 16
#define SHIFT_OPR 21
#define SHIFT_M 28
#define SHIFT_OPC 29
#define SHIFT_SF 31

// Arithmetic and Logical
#define SHIFT_NEGATED 21
#define SHIFT_SHIFT 22

// Multiply
#define SHIFT_RA 10
#define SHIFT_X 15

// Mask
#define MASK(x) ((1<<x) - 1)

typedef enum {
    UNKNOWN,
    ARITHMETIC,
    BIT_LOGIC,
    MULTIPLY
} OpGroup;


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}


