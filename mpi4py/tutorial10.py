
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    A_sendbuf = 2 * np.eye(size, dtype='float64')
    x = np.ones(size, dtype='float64')
else:
    A_sendbuf = None
    x = np.empty(size, dtype='float64')
    
A_recvbuf = np.empty(size, dtype='float64')
comm.Scatter(A_sendbuf, A_recvbuf, root=0)
comm.Bcast(x, root=0)

y_sendbuf = A_recvbuf @ x

if rank == 0:
    y_recvbuf = np.empty(size, dtype='float64')
else:
    y_recvbuf = None

comm.Gather(y_sendbuf, y_recvbuf, root=0)
    
print("s", rank, A_sendbuf)
print("r", rank, A_recvbuf)
print("x", rank, x)
print("y", rank, y_recvbuf)
