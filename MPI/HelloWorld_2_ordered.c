#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv){

	int np, rank;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (int i=0; i<np; i++) {	
	
		MPI_Barrier(MPI_COMM_WORLD);
		
		if (i == rank) {
			printf("np %d, rank %d\n", np, rank);
		}	

	}

	MPI_Finalize();
	return 0;
}

