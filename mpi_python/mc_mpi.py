from mpi4py import MPI
import sys
import random
import time
import math

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

a, b = -10.0, 10.0
N = 1_000_000
if len(sys.argv) > 1:
    N = int(sys.argv[1])
local_N = N // size

random.seed(1234 + rank)

def f(x):
    return (2.0 - x) ** 2

start = time.time()

local_best_x = None
local_best_f = float("inf")

for _ in range(local_N):
    x = random.uniform(a, b)
    fx = f(x)

    if fx < local_best_f:
        local_best_f = fx
        local_best_x = x

# Reduce only the function value
global_best_f = comm.reduce(local_best_f, op=MPI.MIN, root=0)

end = time.time()

if rank == 0:
    print(f"Best f(x) = {global_best_f:.10e}")
    print(f"time = {end - start:.6f} s")

