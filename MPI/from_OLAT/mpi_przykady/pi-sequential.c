#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
int main (int argc, char *argv[]) {
	int N = atoi(argv[1]), i;
	double sum = 0;
	double result = 0;
	float timing;
	MPI_Init(&argc, &argv);
	double start = MPI_Wtime(); 
	for ( i = 0; i <= N; i++) {
		sum += 4/(1 + pow (((i - 0.5)/N), 2));
	}
	result = sum / N;
	double stop = MPI_Wtime();
	MPI_Finalize();
	timing = stop - start;
	printf ("%.17f %f\n", result, timing);
	return 0;
}
