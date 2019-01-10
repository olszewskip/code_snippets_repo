#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char **argv) {
	MPI_Init (&argc, &argv);
	int n = atoi(argv[1]);
	int rank, size;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	int next = (rank + 1) % size;
	int previous = (rank - 1 + size) % size;
	typedef int buffer;
	#define elems 10000 
	MPI_Datatype type = MPI_INT;
	buffer toSend[elems] = {};
	buffer toRecv[elems];
	for (int i = 0; i < n; i++) {
		printf ("Rank %d/%d, round %d, sending\n", rank, size, i);
		MPI_Request request;
		MPI_Isend (&toSend, elems, type, next, 0, MPI_COMM_WORLD, &request);
		printf ("Rank %d/%d, round %d, recving\n", rank, size, i);
		MPI_Recv (&toRecv, elems, type, previous, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf ("Rank %d/%d, round %d, finishing sending\n", rank, size, i);
		MPI_Wait (&request, MPI_STATUS_IGNORE);
	}
	MPI_Finalize();
	return 0;
}
