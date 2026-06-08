#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>
#include <stdbool.h>

#endif 

#define MAX_SYM_LEN 64
#define INITIAL_MAX_SYMTABLE_SIZE 256

typedef char sym_t[MAX_SYM_LEN];

typedef struct {
	sym_t symbol;
	uint32_t addr;
} symkv_t;

struct symtable {
	symkv_t *entries;
	int count;
	int cap;
};

typedef struct symtable *symtable;

// Prototypes for all symtable functions
extern symtable init_table(void);
extern void free_table(symtable table);
extern bool insert_symkv(symtable table, sym_t s, uint32_t address);
extern uint32_t get_addr(symtable table, sym_t s);



