#!/bin/bash
#SBATCH --job-name=mc_openmp
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=48
#SBATCH --time=00:10:00
#SBATCH --output=openmp_%j.out

module load gcc

N=100000000

for t in 1 2 4 8 16 32 48; do
    export OMP_NUM_THREADS=$t
    ./mc_openmp $N
done

