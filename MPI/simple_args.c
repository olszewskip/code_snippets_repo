#include <stdio.h>
int main(int argc, char *argv[]) {
	
	for (int i=0; i<argc; i++) {
	       printf("\t%d\t%s\n", i+1, argv[i]);
	}

	printf("\n");

	return 0;
}
