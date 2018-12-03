#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {

	int rank, size, tag=123;

	int elA;
	int elB;

	int dims[] = { 3, 3 };
	int periods[] = { 1, 1 };

	int A[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
	int B[3][3] = {{10,20,30}, {40,50,60}, {70,80,90}};
	int C[3][3];

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	// MPI_Comm_size (MPI_COMM_WORLD, &size);
	
	MPI_Comm cart;
	MPI_Cart_create (MPI_COMM_WORLD, 2, dims, periods, 1, &cart);


	MPI_Status status;
	MPI_Request request;

	int coords[2];
	MPI_Cart_coords (cart, rank, 2, coords);

	int result = 0;
	elA = A[coords[0]][coords[1]];
	elB = B[coords[0]][coords[1]];

	int where_from_A;
	int where_to_A;
	int where_from_B;
	int where_to_B;

	// Step 1
	MPI_Cart_shift(cart, 0,  coords[0], &rank, &where_from_A);
	MPI_Cart_shift(cart, 0, -coords[0], &rank, &where_to_A);
	printf("step1-przed-A rank-%d where_to_A-%d where_from_A-%d\n", rank, where_to_A, where_from_A);
	MPI_Sendrecv(&elA, 1, MPI_INT, where_to_A, tag,
				 &elA, 1, MPI_INT, where_from_A, tag, MPI_COMM_WORLD, &status);
	//printf("step1 po A\n");

	MPI_Cart_shift(cart, 1,  coords[1], &rank, &where_from_B);
	MPI_Cart_shift(cart, 1, -coords[1], &rank, &where_to_B);
	//printf("step1 przed B\n");
	MPI_Sendrecv(&elB, 1, MPI_INT, where_to_B, tag,
				 &elB, 1, MPI_INT, where_from_B, tag, MPI_COMM_WORLD, &status);
	//printf("step1 po B\n");


	// Step 2
	for (int i=1; i<9; i++)

		MPI_Cart_shift(cart, 0,  1, coords, &where_from_A);
		MPI_Cart_shift(cart, 0, -1, coords, &where_to_A);
		MPI_Sendrecv(&elA, 1, MPI_INT, where_to_A, tag,
					 &elA, 1, MPI_INT, where_from_A, tag, MPI_COMM_WORLD, &status);

		MPI_Cart_shift(cart, 1,  1, coords, &where_from_B);
		MPI_Cart_shift(cart, 1, -1, coords, &where_to_B);
		MPI_Sendrecv(&elB, 1, MPI_INT, where_to_B, tag,
					 &elB, 1, MPI_INT, where_from_B, tag, MPI_COMM_WORLD, &status);

		result += elA * elB;

	C[coords[0]][coords[1]] = result;

	MPI_Finalize ();

	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}

	return 0;
}