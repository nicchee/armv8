#include <stdint.h>

#define MAX_SYM_LEN 64
#define MAX_SYMTABLE_SIZE 1024

typedef char sym_t[MAX_SYM_LEN];

typedef struct {
	sym_t symbol;
	uint32_t addr;
} symkv_t;

typedef struct {
	symkv_t entries[MAX_SYMTABLE_SIZE];
	int count;
} symtable_t;



