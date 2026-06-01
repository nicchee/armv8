#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symtable_t* init_table(void) {
	symtable_t* table = malloc(sizeof(symtable_t));
	if (table != NULL) {
		table->count = 0;
	} else {
		fprintf(stderr, "Symbol table pointer was null")
	}
	return table;
}

void free_table(symtable_t* table) {
	if (table != NULL) {
		free(table);
	}
}

//Insert symbol and address into table
void insert_symkv(sym* s, uint32_t addr, symtable_t* table) {
	//Check if pointers are NULL
	if (table == NULL || s == NULL) {
		fprintf(stderr, "Null pointer");
		exit(1);
	}
	//Check if sym table is full
	if (table->count >= MAX_SYMBOLS) {
		fprintf(stderr, "Symbol table is full");
		exit(1);
	}	

	//Check if symbol alr in table - linear search
	for (int i = 0; i < table->count; i++) {
		if (strcmp(s, table.symbol[i]) == 0) {
			return 0;
		}
	}
	//Symbol not in table -> insert entry
	//Prevent buffer overrun, check if s is too long for buffer
	if (strlen(*s) > (MAX_SYM_LEN - 1) ) {
		fprintf(stderr, "symbol too long to fit in buffer");
		exit(1);
	}
	// s and its null terminator will fit in buffer so can strncpy safely 
	strncpy(table.symbol[i], s, MAX_SYM_LEN); 
	(table->count)++;
	return 0;
}


