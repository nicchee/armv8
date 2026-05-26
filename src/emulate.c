#include <stdlib.h>
#include "emulate.h"

// Position of each instruction part for Single Data Transfer & Load Literal
#define SHIFT_RT 0;
#define SHIFT_XN 5;
#define SHIFT_SIMM19 5;
#define SHIFT_OFFSET 10;
#define SHIFT_L 22;
#define SHIFT_U 24;
#define SHIFT_SF 30;
#define SHIFT_OP 31;

#define MASK(x) ((1<<x) - 1)


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
