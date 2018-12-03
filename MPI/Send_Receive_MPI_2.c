#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {

	int numrank, myrank, dest;
	dest = 0;

	int tag = 13;

	int some_int = 100;

	int suma = 0;

	MPI_Status status;

	MPI_Init(&argc, &argv);
       	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	
	if (myrank != dest) {
		// sprintf(shouted_int, "Hello, Ich bin %d\n", myrank);
		
		MPI_Send(&some_int, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else {
		for (int src = 1; src < numrank; src++) {
			MPI_Recv(&some_int, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
			// printf("%d\n", some_int);
			suma += some_int;
		}

		printf("%d\n", suma);
	}


	MPI_Finalize();

	return 0;
}
