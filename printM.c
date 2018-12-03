#include <stdio.h>

int main(int argc, char **argv)
{
	int M[2][2] = {{1,2}, {3,4}};	

	for (int i = 0; i<2; i++) {
		for (int j=0; j<2; j++) {
			printf("%d ", M[i][j]);
		}
		printf("\n");
	}

	return 0;
}