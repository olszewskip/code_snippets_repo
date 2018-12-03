#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
	int N = atoi(argv[1]), i;
	double sum = 0;
	double result_pi = 0;
	double result_time = 0;

	double start, stop;
	double timing;

	typedef struct {
		double partial_sum;
		double time;
	} partial_sum_with_time;

	partial_sum_with_time partial_result;
	MPI_Datatype mpi_partial_sum_with_time;

	int rank, size;
	int chunk = 0;
	partial_sum_with_time *partials = NULL;
	MPI_Request *requests = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Aint addr_partial_sum, addr_time;
	MPI_Get_address (&partial_result.partial_sum, &addr_partial_sum);
	MPI_Get_address (&partial_result.time, &addr_time);
	int blocklength[] = { 1, 1 };
	MPI_Aint displacements[] = { 0, addr_time - addr_partial_sum};
	MPI_Datatype types[] = { MPI_DOUBLE, MPI_DOUBLE };
	MPI_Type_create_struct (2, blocklength, displacements, types, &mpi_partial_sum_with_time);
	MPI_Type_commit (&mpi_partial_sum_with_time);

	
	if (rank == 0) {
		partials = malloc (sizeof(mpi_partial_sum_with_time)*(size - 1));
		requests = malloc (sizeof(MPI_Request)*(size - 1));
	}
	chunk = N/size;
	
	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Irecv (&partials[i - 1], 1, mpi_partial_sum_with_time, i, 123, MPI_COMM_WORLD, &requests[i - 1]);
		}
	}

	start = MPI_Wtime();
	for ( i = chunk * rank; i < chunk * (rank + 1); i++) {
		sum += 4/(1 + pow (((i + 0.5)/N), 2));
	}
	sum /= N;
	stop = MPI_Wtime();
	timing = stop - start;
	partial_result.partial_sum = sum;
	partial_result.time = timing;

	if (rank == 0) {
		MPI_Waitall(size - 1, requests, MPI_STATUSES_IGNORE);
		result_pi += sum;
		for (int i = 0; i < size - 1; i++) {
			result_pi += partials[i].partial_sum;
			result_time += partials[i].time;
		}	
	} else {
		MPI_Send (&partial_result, 1, mpi_partial_sum_with_time, 0, 123, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	free (partials);
	free (requests);
	if (rank == 0) {
		printf ("%f %f\n", result_pi, result_time);
	}
	return 0;
}
