from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
comm.Barrier()
time0 = MPI.Wtime()
size = comm.Get_size()
rank = comm.Get_rank()

n = 1024 * 4
m = 10
dtype1 = 'int16'
dtype2 = 'int8'

if n % size != 0:
    raise ValueError()

dim1 = int(n // size)

# Read the file into memory at thread 0
# And scatter the resulting vector A
if rank == 0:
    A = []
    with open("in.txt", "r") as file:
        for line in file:
            A.append(int(line.strip()))
    A_sendbuf = np.array(A, dtype1)
else:
    A_sendbuf = None

A_recvbuf = np.empty(dim1, dtype1)
comm.Scatter(A_sendbuf, A_recvbuf, root=0)

# Compute the partial histogram at each thread
B_sendbuf = np.zeros(m + 1, dtype2)
for num in A_recvbuf:
    B_sendbuf[num] += 1

# Reduce the histograms
B_recvbuf = np.empty(m + 1, dtype2)
comm.Reduce(B_sendbuf, B_recvbuf, op=MPI.SUM, root=0)

# Print the result to screen
# Stop the timer
if rank == 0:
    print(B_recvbuf)
    time1 = MPI.Wtime()
    print(f"n={size}. Finished in {round(time1-time0, 4)} sec.")
