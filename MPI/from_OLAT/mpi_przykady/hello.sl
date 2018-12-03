#!/bin/bash -l
#SBATCH --time=00:10:0
#SBATCH --nodes=1
#SBATCH --ntasks-per-node 4
#SBATCH --job-name="Hello_mpi"
#SBATCH --output="hello.out"
#SBATCH --error="hello.err"
mpiexec ./a.out
