#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {

	int numrank, dest, myrank;
	dest = 0;

	int tag = 13;
	char message[50];

	MPI_Status status;

	MPI_Init(&argc, &argv);
       	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	
	if (myrank != dest) {
		sprintf(message, "Hello, Ich bin %d\n", myrank);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	else {
		for (int src = 1; src < numrank; src++) {
			MPI_Recv(message, sizeof(message), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
		printf("%s", message);
		}

		printf("Hello, Ich bin %d\n", myrank);
	}

	if (myrank == 0) {
		
	}

	MPI_Finalize();

	return 0;
}
