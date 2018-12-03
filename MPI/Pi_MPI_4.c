// does not work

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char** argv) {

	int size, rank;
	int tag = 13;

	MPI_Status status;

        int N = atoi(argv[1]);

	MPI_Init(&argc, &argv);
       	MPI_Comm_rank(MPI_COMM_WORLD, &size);
	MPI_Comm_size(MPI_COMM_WORLD, &rank);
	

	MPI_Request requests[40];

        int n = N / size ;
        int i0 = rank * n;
        int i1 = (rank + 1) * n;
	
	double pi = 0;
	double pi_part = 0;
	double pi_parts[40];
	
	if (rank != 0) {
		for (int i=i0; i<i1; i++) {
	                pi_part += 1 / (1 + pow( (((double)i+0.5) / N), 2) );
		}

		MPI_Send(&pi_part, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

	}
	else {
		for (int src = 1; src < size; src++) {
			MPI_Irecv(&pi_parts[src], 1, MPI_INT, src, tag, MPI_COMM_WORLD, &requests[src]);
		}

		for (int i=i0; i<i1; i++) {
	                pi += 1 / (1 + pow( (((double)i+0.5) / N), 2) );
		}
		
		MPI_Waitall(size, requests, MPI_STATUSES_IGNORE);

		for (int i=0; i<size; i++) {
			pi += pi_parts[i];
		}

		printf("%f\n", pi);

	}

	MPI_Finalize();	

	return 0;
}
