#include <mpi.h>
#include <stdio.h>
int main (int argc, char *argv[]) {
	int rank, size;
	MPI_Init (&argc, &argv);
	int local_array[2];
	int result[2];
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	local_array[0] = rank;
	local_array[1] = 1;

	MPI_Reduce (&local_array, &result, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf ("%d : %d\n", result[0], result[1]);
	}
	MPI_Finalize ();
	return 0;
}
