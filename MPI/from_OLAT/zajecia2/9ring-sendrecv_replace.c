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
	#define elems 1000000 
	MPI_Datatype type = MPI_INT;
	buffer toSend[elems] = {};
	for (int i = 0; i < n; i++) {
		printf ("Rank %d/%d, round %d, sending&recving\n", rank, size, i);
		MPI_Sendrecv_replace (&toSend, elems, type, next, 0, 
		previous, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	MPI_Finalize();
	return 0;
}
