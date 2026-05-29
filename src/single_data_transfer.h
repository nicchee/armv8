#ifndef SINGLE_DATA_TRANSFER_H
#define SINGLE_DATA_TRANSFER_H

extern void single_data_transfer(uint32_t instruction, mstate_t *state);

// Masking function - generate the mask bits required
#define MASK(x) ((1U << (x)) - 1)

#endif
