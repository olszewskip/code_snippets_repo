
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

sendbuf = None

if rank == 0:
    sendbuf = np.empty([size, 10], dtype='int32')
    sendbuf.T[:,:] = range(size)
    
print(rank, sendbuf)

recvbuf = np.empty(10, dtype='int32')
comm.Scatter(sendbuf, recvbuf, root=0)
assert np.allclose(recvbuf, rank)

print(rank, recvbuf)
