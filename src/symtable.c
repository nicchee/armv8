#include "symtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool null_terminated(char *s, int max) {
	for (int i = 0; i < max; i++) {
		if (s[i] == '\0') { return true; }
	}
	return false;
}

//Create symbol table, return pointer
symtable init_table(void) {
	symtable table = malloc(sizeof(struct symtable));
	if (table == NULL) { 
		fprintf(stderr, "Symbol table malloc failed\n");
		exit(EXIT_FAILURE);
	}
	table->cap = INITIAL_MAX_SYMTABLE_SIZE;
	table->count = 0;
	table->entries = malloc(table->cap * sizeof(symkv_t));
	if (table->entries == NULL) { 
		fprintf(stderr, "Symbol table entries malloc failed\n");
		exit(EXIT_FAILURE);
	}
	return table;
}

//Free memory
void free_table(symtable table) {
	if (table != NULL) {
		if (table->entries != NULL) {
			free(table->entries);
			table->entries = NULL;
		}
		free(table);
	}	
}

//bool success = insert_symkv(table, s, address);
//	Insert symbol and address into table
bool insert_symkv(symtable table, sym_t s, uint32_t address) {
	// check pointers are NOT NULL
	assert(table != NULL && s != NULL);

	// check s is NULL terminated
	assert(null_terminated(s, MAX_SYM_LEN));

	// Grow if table is full
	if (table->count >= table->cap) {
		table->cap += 100;
		table->entries = realloc(table->entries, table->cap * sizeof(symkv_t));
	}	

	//Check if symbol already in table - linear search
	for (int i = 0; i < table->count; i++) {
		if (strcmp( s, table->entries[i].symbol) == 0) {
			fprintf(stderr, "Duplicate symbol definition\n");
			return false;
		}
	}

	//Symbol not in table -> insert entry
	int currIndex = table->count;
	//Prevent buffer overrun, check if s is too long for buffer
	if (strlen(s) > (MAX_SYM_LEN - 1) ) {
		fprintf(stderr, "Symbol too long to fit in buffer\n");
		return false;
	}

	// s and its null terminator will fit in buffer so can strncpy safely and return
	strcpy(table->entries[currIndex].symbol, s);
	table->entries[currIndex].addr = address;
	table->count++;
	return true;
}

//Lookup symbol in table, return address
uint32_t get_addr(symtable table, sym_t s) {
	//int currIndex = table->count; - if no bug then delete
	for (int i = 0; i < table->count; i++) {
		if (strcmp(s, table->entries[i].symbol) == 0) {
			return table->entries[i].addr;
		}
	}
	fprintf(stderr, "Couldn't find symbol in table");
	exit(1);

}
