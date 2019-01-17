from mpi4py import MPI
import numpy as np
import time
import sys


comm = MPI.COMM_WORLD
comm.Barrier()
t_start = MPI.Wtime()

n=1048576
rank = comm.Get_rank()
size = comm.Get_size()

if n % size != 0:
    raise ValueError(f"Vector of length {n} needs to be split evenly!")

m = n // size

B_chunk = np.random.rand(m)
C_chunk = np.random.rand(m)
A_chunk = B_chunk * C_chunk
sum_chunk = np.sum(A_chunk)
sum_ = 0

comm.Reduce(
    [sum_chunk, MPI.DOUBLE],
    [sum_, MPI.DOUBLE],
    op = MPI.SUM,
    root = 0
)



comm.Barrier()
t_end = MPI.Wtime()
if rank == 0:
    print(f"Suma: {sum_}.")
    print(f"Finished in {t_end - t_start} sec.")
