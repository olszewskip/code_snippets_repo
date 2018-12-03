#include <mpi.h>
#include <stdio.h>

int gcd (int a, int b) {
    if (a == 0) 
        return b; 
    return gcd(b % a, a); 
}
void Gcd (void *a, void *b, int *len, MPI_Datatype *datatype) {
	int *ia = (int*)a;
	int *ib = (int*)b;
	for (int i = 0; i < *len; i++) {
		ib[i] = gcd(ia[i], ib[i]);
	}
}
int main (int argc, char *argv[]) {
	int rank, size;
	MPI_Init (&argc, &argv);
	int local_array[2];
	int result[2];
	MPI_Op custom_operation;

	
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	local_array[0] = rank;
	local_array[1] = 2 * rank * rank;

	MPI_Op_create (Gcd, 1, &custom_operation);
	MPI_Reduce (local_array, (rank == 3) ? result : NULL, 2, MPI_INT, custom_operation, 3, MPI_COMM_WORLD);
	if (rank == 3) {
		printf ("%d : %d\n", result[0], result[1]);
	}
	MPI_Op_free (&custom_operation);
	MPI_Finalize ();
	return 0;
}
