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


        typedef struct {
		double result;
		double time;
        } result_struct;

	result_struct *partials = NULL;
	MPI_Request *requests = NULL;
	MPI_Status *statuses = NULL;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		partials = malloc (sizeof(result_struct)*(size - 1));
		requests = malloc (sizeof(MPI_Request)*(size - 1));
		statuses = malloc (sizeof(MPI_Status)*(size - 1));
	}
	chunk = N/size;
	start = MPI_Wtime();
	result_struct example;

        MPI_Datatype mpi_result_struct;

        MPI_Aint addr_result, addr_time;
        MPI_Get_address (&example.result, &addr_result);
        MPI_Get_address (&example.time, &addr_time);
        int blocklength[] = { 1, 1 };
        MPI_Aint displacements[] = { 0, addr_time - addr_result};
        MPI_Datatype types[] = { MPI_DOUBLE, MPI_DOUBLE };
        MPI_Type_create_struct (2, blocklength, displacements, types, &mpi_result_struct);
        MPI_Type_commit (&mpi_result_struct);

	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Irecv (&partials[i - 1], 1, mpi_result_struct, i, 0, MPI_COMM_WORLD, &requests[i - 1]);
		}
	}
	for ( i = chunk * rank; i < chunk * (rank + 1); i++) {
		sum += 4/(1 + pow (((i + 0.5)/N), 2));
	}
	sum /= N;
	example.result = sum;
	example.time = MPI_Wtime() - start;
	if (rank != 0) {
		MPI_Send (&example, 1, mpi_result_struct, 0, 0, MPI_COMM_WORLD);
	} else {
		MPI_Waitall(size - 1, requests, MPI_STATUSES_IGNORE);
		result = sum;
		for (int i = 0; i < size - 1; i++) {
			result += partials[i].result;
		}
		stop = MPI_Wtime();
		timing = stop - start; 
	}
	MPI_Type_free(&mpi_result_struct);
	MPI_Finalize();
	free (partials);
	free (requests);
	free (statuses);
	if (rank == 0) {
		printf ("%f %f\n", result, timing);
	}
	return 0;
}
