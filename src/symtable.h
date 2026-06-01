#define MAX_SYM_LEN 64
#define MAX_SYMTABLE_SIZE 1024
typedef char sym[MAX_SYM_LEN];

typedef struct {
	sym symbol;
	uint32_t addr;
} symkv_t;

typedef struct {
	symkv_t[MAX_SYMTABLE_SIZE];
	int symCount = 0;
} symtable_t;



