#!/bin/bash
#SBATCH --job-name=mc_mpi_py
#SBATCH --ntasks=112
#SBATCH --time=00:10:00
#SBATCH --output=mpi_py_%j.out

module load openmpi python

N=100000000

for p in 1 2 4 8 16 32 64 112; do
    srun -n $p python3 mc_mpi.py $N
done

