#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
	int N = atoi(argv[1]), i;
	double sum = 0, result = 0;
	double start, stop;
	double timing;

	int rank, size;
	int chunk = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	chunk = N/size;
	start = MPI_Wtime();
	for ( i = chunk * rank; i < chunk * (rank + 1); i++) {
		sum += 4/(1 + pow (((i + 0.5)/N), 2));
	}
	sum /= N;
	MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	stop = MPI_Wtime();
	timing = stop - start;
	MPI_Finalize();
	if (rank == 0) {
		printf ("%f %f\n", result, timing);
	}
	return 0;
}
