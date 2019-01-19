from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
comm.Barrier()
time0 = MPI.Wtime()
size = comm.Get_size()
rank = comm.Get_rank()

n = 1048576
dtype = 'float32'

if n % size != 0:
    raise ValueError()

dim1 = int(n // size)

# Initialize random vectors A and B at one thread
# and scatter them to all threads
if rank == 0:
    A_sendbuf = np.random.rand(n).astype(dtype)
    B_sendbuf = np.random.rand(n).astype(dtype)
else:
    A_sendbuf = None
    B_sendbuf = None

A_recvbuf = np.empty(dim1, dtype)
B_recvbuf = np.empty(dim1, dtype)

comm.Scatter(A_sendbuf, A_recvbuf, root=0)
comm.Scatter(B_sendbuf, B_recvbuf, root=0)

# Compute part of the sum in the definition of the dot product
part_dotproduct = np.sum(A_recvbuf * B_recvbuf).reshape(1)

# Reduce the parts back together using addition at thread 0
dotproduct = np.empty(1, dtype)
comm.Reduce(part_dotproduct, dotproduct, op=MPI.SUM, root=0)

# Print the result to screen
# Stop the timer
if rank == 0:
    print(f"The dot product equals {dotproduct[0]}")
    time1 = MPI.Wtime()
    print(f"n={size}. Finished in {round(time1-time0, 4)} sec.")
