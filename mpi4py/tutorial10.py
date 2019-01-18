
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
comm.Barrier()
time0 = MPI.Wtime()

rank = comm.Get_rank()
size = comm.Get_size()

n = 4
dtype = "float32"

if n % size != 0:
    raise ValueError("The number of processes n needs to be a power of 2.")

dim1 = int(n // size)

# Create random a vector A at one thread
if rank == 0:
    A_sendbuf = np.random.rand(n).astype(dtype)
else:
    A_sendbuf = None

# and scatter A to all threads
A_recvbuf = np.empty(dim1, dtype)
comm.Scatter(A_sendbuf, A_recvbuf, root=0)

print(rank, A_recvbuf)

part_sum = np.sum(A_recvbuf).reshape(1)
sum_ = np.empty(1, dtype)
comm.Reduce(part_sum, sum_, root=0)

if rank == 0:
    print(sum_)
