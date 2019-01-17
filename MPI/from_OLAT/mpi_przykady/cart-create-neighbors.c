#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
  int rank;
  MPI_Comm cart;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int dims[] = {4, 3};
  int periods[] = {1, 0};
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart);

  int coords[2];
  for (int i = 0; i < 12; i++) {
    if (i == rank) {
      int computed;
      int dest;
      MPI_Cart_coords(cart, rank, 2, coords);
      printf("I am process no. %d and my coordinates are (%d, %d)\n", i,
             coords[0], coords[1]);
      MPI_Cart_rank(cart, coords, &computed);
      printf("Dually, my computed rank is %d\n", computed);
      MPI_Cart_shift(cart, 1, -1, coords, &dest);
      if (dest == MPI_PROC_NULL) {
        printf("I have no neighbor :(\n");
      } else {
        printf("My upmost neighbor is (%d)\n", dest);
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
