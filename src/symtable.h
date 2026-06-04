#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>
#include <stdbool.h>

#endif 

#define MAX_SYM_LEN 64
#define MAX_SYMTABLE_SIZE 1024

typedef char sym_t[MAX_SYM_LEN];

typedef struct {
	sym_t symbol;
	uint32_t addr;
} symkv_t;

struct symtable {
	symkv_t entries[MAX_SYMTABLE_SIZE];
	int count;
};

typedef struct symtable *symtable_t;

// Prototypes for all symtable functions
extern symtable_t init_table(void);
extern void free_table(symtable_t table);
extern bool insert_symkv(symtable_t table, sym_t *s, uint32_t address);
extern uint32_t get_addr(symtable_t table, sym_t *s);



