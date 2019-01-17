#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  int rank, size, src;
  int number;
  int sum = 0;
  int tag = 42;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank != 0) {
    MPI_Send(&rank, 1, MPI_INT, 0, tag,
             MPI_COMM_WORLD);
  } else {
    for (int src = 1; src < size; src++) {
      MPI_Recv(&number, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
      sum += number;
    }
    printf("%d", sum);
  }
  MPI_Finalize();
  return 0;
}
