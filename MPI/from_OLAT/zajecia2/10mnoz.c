#include <stdio.h>
#define N 3
int main (int argc, char *argv[]) {
	int m1[N][N] = { { 1, 2, 3 },
		   { 4, 5, 6},
		   { 0, 1, 0 }};
	int m2[N][N] = { { 1, 2, 3},
		       { 0, 0, 0},
		       { 3, 2, 1} };

	int c[N][N] = { 0 };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				c[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf ("%d ", c[i][j]);
		}
		printf ("\n");
	}
	return 0;
}
