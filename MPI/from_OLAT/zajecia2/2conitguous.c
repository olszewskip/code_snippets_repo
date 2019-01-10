#include <mpi.h>
#include <stdio.h>
int main (int argc, char *argv[]) {
	int rank;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Datatype int4;
	MPI_Type_contiguous (4, MPI_INT, &int4);
	MPI_Type_commit(&int4);	
	if (rank == 0) {
		int b[] = {1, 2, 3 ,4};
		MPI_Send (b, 1, int4, 1, 123, MPI_COMM_WORLD);
	} else {
		int b[4];
		MPI_Recv (b, 1, int4, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < 4; i++) {
			printf ("%d ", b[i]);
		}
		printf ("\n");
	}
	MPI_Type_free (&int4);
	MPI_Finalize ();
	return 0;
}
