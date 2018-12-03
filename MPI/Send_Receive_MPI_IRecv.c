#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {

	int numrank, myrank;

	int tag = 13;
	char message[50];

	MPI_Status status;
	MPI_Request request;

	MPI_Init(&argc, &argv);
       	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	
	if (myrank != 0) {
		sprintf(message, "Hello, Ich bin %d\n", myrank);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	else {
		for (int src = 1; src < numrank; src++) {
			printf("Inicjalizacja komunikacji\n");
			MPI_Irecv(message, sizeof(message), MPI_CHAR, src, tag, MPI_COMM_WORLD, &request);
			printf("Rozpoczęto odbieranie\n");
			MPI_Wait(&request, &status);
			printf("%s", message);
		}

	}

	MPI_Finalize();

	return 0;
}
