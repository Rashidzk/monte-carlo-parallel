#!/bin/bash
#SBATCH --job-name=mc_mpi_c
#SBATCH --ntasks=112
#SBATCH --time=00:10:00
#SBATCH --output=mpi_c_%j.out

module load gcc openmpi

N=100000000

for p in 1 2 4 8 16 32 64 112; do
    srun -n $p ./mc_mpi $N
done

