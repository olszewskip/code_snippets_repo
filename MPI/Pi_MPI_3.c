// niedokonczone...

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
	
	int rank, size;

	MPI_Init (&argc, &argv);
	
	double t0 = MPI_Wtime();

	double pi;

	int N = atoi(argv[1]);
	
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	int n = N / size ;
	int i0 = rank * n;
	int i1 = (rank + 1) * n;

	double pi_part = 0;

	MPI_Request request;

	MPI_Request *req_tab = malloc(size*sizeof(MPI_Request));
	
	if (rank == 0) {
		
		double received;

		MPI_Irecv(&received, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, req_tab);
		
		for (int i=i0; i<i1; i++) {		
			pi_part += 1 / (1 + pow( (((double)i+0.5) / N), 2) );
		}

		MPI_Waitall(size, req_tab, MPI_STATUS_IGNORE);
		
		pi += ??
	
	}
	else {
		for (int i=i0; i<i1; i++) {		
			pi_part += 1 / (1 + pow( (((double)i+0.5) / N), 2) );
		}

		MPI_Isend(&pi_part, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, &request); 
		
	}
		
	
	
	free(req_tab);

	double t1 = MPI_Wtime();

	MPI_Finalize();
	
	if (rank == 0) {

		pi = 4 * pi / n / size;
	
		printf("pi = %.14lf\n", pi);
		printf("time %f s\n", t1-t0);
	}	
	
	return 0;
}

