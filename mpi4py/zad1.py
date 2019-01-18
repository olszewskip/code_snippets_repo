from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
comm.Barrier()
time0 = MPI.Wtime()
rank = comm.Get_rank()
size = comm.Get_size()

n = 1048576
dtype = "float32"

if n % size != 0:
    raise ValueError("The number of processes n needs to be a power of 2.")

dim0 = size
dim1 = int(n / size)

if rank == 0:
    
    B = np.random.rand(n).astype(dtype)
    B_sendbuf = B.reshape(dim0, dim1)
    
    C = np.random.rand(n).astype(dtype)
    C_sendbuf = C.reshape(dim0, dim1)
    
else:
    B_sendbuf = None
    C_sendbuf = None
    
B_recvbuf = np.empty(dim1, dtype)
comm.Scatter(B_sendbuf, B_recvbuf, root=0)
print(f"{rank}) {B_recvbuf}")

C_recvbuf = np.empty(dim1, dtype)
comm.Scatter(C_sendbuf, C_recvbuf, root=0)
print(f"{rank}) {C_recvbuf}")

# the main operation
A_sendbuf = B_recvbuf + C_recvbuf
print(f"{rank}) {A_sendbuf}")

if rank == 0:
    A_recvbuf = np.empty((dim0, dim1), dtype)
else:
    A_recvbuf = None
    
comm.Gather(A_sendbuf, A_recvbuf, root=0)

if rank == 0:
    print(f"{rank}) {A_recvbuf}")
    A = A_recvbuf.reshape(n)
    print(f"A[0]={A[0]}, A[-1]={A[-1]}")
    time1 = MPI.Wtime()
    print(f"n={size}. Finished in {round(time1-time0, 4)} sec.")
    
#comm.Barrier()
