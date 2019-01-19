from mpi4py import MPI
import numpy

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

# passing MPI datatypes explicitly
if rank == 0:
    data = numpy.arange(20, dtype='i')
    print('a', rank, data, data.dtype)
    comm.Send([data, MPI.INT], dest=1, tag=77)
elif rank == 1:
    data = numpy.empty(20, dtype='i')
    comm.Recv([data, MPI.INT], source=0, tag=77)
    print('b', rank, data, data.dtype)
    

# automatic MPI datatype discovery
if rank == 0:
    data = numpy.arange(10, dtype=numpy.float64)
    print('a', rank, data, data.dtype)
    comm.Send(data, dest=1, tag=13)
elif rank == 1:
    data = numpy.empty(10, dtype=numpy.float64)
    comm.Recv(data, source=0, tag=13)
    print('b', rank, data, data.dtype)