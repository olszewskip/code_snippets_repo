#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {

	int numrank, myrank, dest;
	dest = 0;

	int tag = 13;

	int some_int = 100;
	int some_ints[2] = {1,2};

	int suma = 0;
	int suma_0 = 0;
	int suma_1 = 0;

	MPI_Status status;

	MPI_Init(&argc, &argv);
       	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	
	if (myrank != dest) {
		// sprintf(shouted_int, "Hello, Ich bin %d\n", myrank);
		
		MPI_Send(&some_ints, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else {
		for (int src = 1; src < numrank; src++) {
			MPI_Recv(&some_ints, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			printf("%d\n", some_int);
			suma_0 += some_ints[0];
			suma_1 += some_ints[1];
		}

		printf("%d\t%d\n", suma_0, suma_1);
	}


	MPI_Finalize();

	return 0;
}
