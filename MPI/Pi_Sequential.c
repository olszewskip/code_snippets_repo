#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
	
	double t0 = MPI_Wtime();

	int N = atoi(argv[1]);
	double pi = 0;
	for (int i=0; i<N; i++) {
		
		pi += 1 / (1 + pow( (((double)i+0.5) / N), 2) );

	}

	pi = 4 * pi / N;
	
	double t1 = MPI_Wtime();

	//MPI_Init(&argc, &argv);
	printf("pi = %.14lf\n", pi);
	printf("time %f s\n", t1-t0); 
	//MPI_Finalize();

	return 0;
}

