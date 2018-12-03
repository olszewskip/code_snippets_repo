#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
	
	double t0 = MPI_Wtime();
	int rank, size;

	MPI_Init (&argc, &argv);
	
	double pi;

	int N = atoi(argv[1]);
	
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	int n = N / size ;
	int i0 = rank * n;
	int i1 = (rank + 1) * n;

	double pi_part = 0;

	for (int i=i0; i<i1; i++) {
		
		pi_part += 1 / (1 + pow( (((double)i+0.5) / N), 2) );

	}
	
	MPI_Reduce(&pi_part, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
	
	MPI_Finalize();
	
	if (rank == 0) {

		pi = 4 * pi / n / size;
		double t1 = MPI_Wtime();
	
		printf("pi = %.14lf\n", pi);
		printf("time %f s\n", t1-t0);
	}	
	
	return 0;
}

