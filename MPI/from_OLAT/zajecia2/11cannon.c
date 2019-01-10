#include <mpi.h>
#include <stdio.h>
#include <assert.h>

#define SIZE 3
int main (int argc, char *argv[]) {
        MPI_Init (&argc, &argv);


        int a[SIZE][SIZE] = { { 1, 2, 3},
                        { 3, 2, 1},
                        {2, 1, 3 } };
        int b[SIZE][SIZE] = { { 1, 1, 0},
                        {1, 0, 1},
                        {0, 1, 1}};
        int size, rank;
        MPI_Comm_size (MPI_COMM_WORLD, &size);
        MPI_Comm_rank (MPI_COMM_WORLD, &rank);
        assert (size == SIZE * SIZE);

        int dims[] = { SIZE, SIZE };
        int periods[] = { 1, 1 };
        MPI_Comm cart;

        MPI_Cart_create (MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
        int my_coords[2];
        MPI_Cart_coords (cart, rank, 2, my_coords);
        int i = my_coords[0], j = my_coords[1];
        int mya = a[i][j],
            myb = b[i][j],
            myc = 0;
        int neighbor_send, neighbor_recv;

        MPI_Cart_shift (cart, 1, -i, &neighbor_recv, &neighbor_send);

	MPI_Sendrecv_replace (&mya, 1, MPI_INT, neighbor_send, 2,
        neighbor_recv, 2, cart, MPI_STATUS_IGNORE);
        
	MPI_Cart_shift (cart, 0, -j, &neighbor_recv, &neighbor_send);
        
        MPI_Sendrecv_replace (&myb, 1, MPI_INT, neighbor_send, 3,
        neighbor_recv, 3, cart, MPI_STATUS_IGNORE);

        

        int na_send, nb_send, na_recv, nb_recv;
        MPI_Cart_shift (cart, 1, -1, &na_recv, &na_send);
        
	MPI_Cart_shift (cart, 0, -1, &nb_recv, &nb_send);
        
	for (int i = 0; i < SIZE; i++) {
                myc += mya * myb;
                MPI_Sendrecv_replace (&mya, 1, MPI_INT, na_send, 0, 
                na_recv, 0, cart, MPI_STATUS_IGNORE);
                MPI_Sendrecv_replace (&myb, 1, MPI_INT, nb_send, 1, 
                nb_recv, 1, cart, MPI_STATUS_IGNORE);
        }

        printf ("c[%d][%d] = %d\n", i, j, myc);
        MPI_Finalize ();
        return 0;
}
