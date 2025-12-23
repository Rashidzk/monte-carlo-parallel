#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <float.h>
#include <math.h>

double f(double x) {
    return (2.0 - x) * (2.0 - x);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double a = -10.0;
    const double b = 10.0;
    long N = 1000000;
if (argc > 1) {
    N = atol(argv[1]);
}

    long local_N = N / size;

    unsigned int seed = 1234 + rank;

    double local_best_x = 0.0;
    double local_best_f = DBL_MAX;

    double start = MPI_Wtime();

    for (long i = 0; i < local_N; i++) {
        double r = (double)rand_r(&seed) / RAND_MAX;
        double x = a + (b - a) * r;
        double fx = f(x);

        if (fx < local_best_f) {
            local_best_f = fx;
            local_best_x = x;
        }
    }

    struct {
        double value;
        int rank;
    } local, global;

    local.value = local_best_f;
    local.rank = rank;

    MPI_Reduce(&local, &global, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Best f(x) = %.10e\n", global.value);
        printf("time = %.6f s\n", end - start);
    }

    MPI_Finalize();
    return 0;
}

