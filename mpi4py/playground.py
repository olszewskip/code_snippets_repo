from mpi4py import MPI
import numpy as np
import sys

time1 = MPI.Wtime()
n=1048576 * 2
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if n % size != 0:
    raise ValueError(f"Vector of length {n} needs to be split evenly!")

m = n // size

B_chunk = np.random.rand(m)
C_chunk = np.random.rand(m)
A_chunk = B_chunk + C_chunk

if rank == 0:
    print(A_chunk[0])
elif rank == size-1:
    print(A_chunk[-1])

sys.stdout.flush()
comm.Barrier()
time2 = MPI.Wtime()
if rank == 0:
    print(f"Finished in {time2 - time1} sec.")
