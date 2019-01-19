from mpi4py import MPI
import numpy as np
np.random.seed(123)

comm = MPI.COMM_WORLD
comm.Barrier()
time0 = MPI.Wtime()
rank = comm.Get_rank()
size = comm.Get_size()

n = 2**24 # 2**20
dtype = "float64"

if n % size != 0:
    raise ValueError("The number of processes n needs to be a power of 2.")

dim1 = int(n // size)

# Create random vectors B, C at one thread
if rank == 0:
    B_sendbuf = np.random.rand(n).astype(dtype)    
    C_sendbuf = np.random.rand(n).astype(dtype)
else:
    B_sendbuf = None
    C_sendbuf = None   

# Scatter B and C to all threads
B_recvbuf = np.empty(dim1, dtype)
C_recvbuf = np.empty(dim1, dtype)

comm.Scatter(B_sendbuf, B_recvbuf, root=0)
comm.Scatter(C_sendbuf, C_recvbuf, root=0)

# Do A = B + C piecewise at each thread
A_sendbuf = B_recvbuf + C_recvbuf

# Gather pieces of A at thread 0
if rank == 0:
    A_recvbuf = np.empty(n, dtype)
else:
    A_recvbuf = None
    
comm.Gather(A_sendbuf, A_recvbuf, root=0)

# Print first and last element of A to screen
# Stop the timer
if rank == 0:
    A = A_recvbuf
    print("A[0]={0}, A[-1]={1}".format(A[0], A[1]))
    time1 = MPI.Wtime()
    print("n={size}. Finished in {time_diff} sec.".format(size=size, time_diff=round(time1-time0, 4)))