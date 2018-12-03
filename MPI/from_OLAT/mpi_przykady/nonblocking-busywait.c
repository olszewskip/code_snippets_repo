#include <mpi.h>
#include <stdio.h>
int main (int argc, char *argv[]) {

	int rank, size;
	MPI_Request request;
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	if (rank == 0) {
		int toSend = 42;
		MPI_Isend (&toSend, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &request);
		printf ("%d started sending...\n", rank);
		MPI_Wait (&request, MPI_STATUS_IGNORE);
	} else {
		int received = 0;
		int flag = 0;
		MPI_Irecv (&received, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		while (!flag) {
			int val = received;
			MPI_Test (&request, &flag, MPI_STATUS_IGNORE);
			printf ("Testing..., val = %d\n", val);
		}
		printf ("Value = %d\n", received);
	}
	MPI_Finalize();
	return 0;
}
