#ifndef TWO_PASS_H
#define TWO_PASS_H

#include "symtable.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern void first_pass(FILE *in, symtable table);
extern void second_pass(FILE *in, symtable table, const char *out_name);

#endif
