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
	double *partials = NULL;
	MPI_Request *requests = NULL;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank == 0) {
		partials = malloc (sizeof(double)*(size - 1));
		requests = malloc (sizeof(MPI_Request)*(size - 1));
	}
	chunk = N/size;
	start = MPI_Wtime();

	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Irecv (&partials[i - 1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &requests[i - 1]);
		}
	}
	for ( i = chunk * rank; i < chunk * (rank + 1); i++) {
		sum += 4/(1 + pow (((i + 0.5)/N), 2));
	}
	sum = sum / N;
	if (rank == 0) {
		MPI_Waitall(size - 1, &requests[0], MPI_STATUSES_IGNORE);
		result = sum;
		for (int i = 0; i < size - 1; i++) {
			result += partials[i];
		}	
	} else {
		MPI_Send (&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		MPI_Send (&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		MPI_Send (&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	stop = MPI_Wtime();
	timing = stop - start;
	MPI_Finalize();
	free (partials);
	free (requests);
	if (rank == 0) {
		printf ("%f %f\n", result, timing);
	}
	return 0;
}
