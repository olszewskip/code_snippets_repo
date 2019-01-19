from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
    data = np.arange(10, dtype='int64')
else:
    data = np.empty(10, dtype='int64')
    
# print('a', rank, data)
    
comm.Bcast(data, root=0)
for i in range(10):
    assert data[i] == i

print('b', rank, data, data.dtype)