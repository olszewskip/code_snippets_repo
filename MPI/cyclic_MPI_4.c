#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define big 9999
int main (int argc, char *argv[]) {
	int rank, size;

	int tag = 42;

	//int big = 9999;

	int tablica[big];

	MPI_Status status;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	int send_to = (rank+1) % size;
	int recv_from = (size+rank-1) % size;

	MPI_Request request;

	MPI_Isend (&tablica, big, MPI_INT, send_to, tag, MPI_COMM_WORLD, &request);

	MPI_Recv (&tablica, big, MPI_INT, recv_from, tag, MPI_COMM_WORLD, &status);
	
	MPI_Finalize ();

	return 0;
}