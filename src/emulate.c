#include <stdlib.h>
#include "emulate.h"

int main(int argc, char **argv) {
	if (argc > 3 || argc < 2) {
		fprintf(stderr, "There must be 1 or 2 arguments passed to %s", argv[0]);
		return EXIT_FAILURE;
	} 
	init();
	bin_file_load(memory, argv[1]);

  return EXIT_SUCCESS;
}
