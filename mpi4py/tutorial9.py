
from mpi4py import MPI
import numpy as np

time0 = MPI.Wtime()

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

sendbuf = np.zeros(10, dtype='int32') + rank
recvbuf = None
if rank == 0:
    recvbuf = np.empty((size, 10), dtype='int32')
    
comm.Gather(sendbuf, recvbuf, root=0)
if rank == 0:
    for i in range(size):
        assert np.allclose(recvbuf[i,:], i)
        
time1 = MPI.Wtime()
        
print(rank, recvbuf)
print()