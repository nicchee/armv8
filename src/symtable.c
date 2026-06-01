#include "symtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Create symbol table, return pointer
symtable_t* init_table(void) {
	symtable_t* table = malloc(sizeof(symtable_t));
	if (table != NULL) {
		table->count = 0;
	} else {
		fprintf(stderr, "Symbol table pointer was null");
	}
	return table;
}

//Free memory
void free_table(symtable_t* table) {
	if (table != NULL) {
		free(table);
	}
}

//Insert symbol and address into table
bool insert_symkv(symtable_t* table, sym_t* s, uint32_t address) {
	//Check if pointers are NULL
	if (table == NULL || s == NULL) {
		fprintf(stderr, "Null pointer");
		return false;
	}
	//Check if sym table is full
	if (table->count >= MAX_SYMTABLE_SIZE) {
		fprintf(stderr, "Symbol table is full");
		return false;
	}	

	//Check if symbol alr in table - linear search
	for (int i = 0; i < table->count; i++) {
		if (strcmp((const char*) s, table->entries[i].symbol) == 0) {
			fprintf(stderr, "Duplicate symbol definition");
			return false;
		}
	}
	//Symbol not in table -> insert entry
	int currIndex = table->count;
	//Prevent buffer overrun, check if s is too long for buffer
	if (strlen(*s) > (MAX_SYM_LEN - 1) ) {
		fprintf(stderr, "Symbol too long to fit in buffer");
		return false;
	}
	// s and its null terminator will fit in buffer so can strncpy safely and return
	strncpy(table->entries[currIndex].symbol, (const char*) s, MAX_SYM_LEN);
	table->entries[currIndex].addr = address;
	(table->count)++;
	return true;
}

//Lookup symbol in table, return address
uint32_t get_addr(symtable_t *table, sym_t s) {
	int currIndex = table->count;
	for (int i = 0; i < table->count; i++) {
		if (strcmp((const char*) s, table->entries[currIndex].symbol) == 0) {
			return table->entries[currIndex].addr;
		}
	}
	fprintf(stderr, "Couldn't find symbol in table");
	exit(1);

}
