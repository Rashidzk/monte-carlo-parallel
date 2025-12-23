#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <math.h>

double f(double x) {
    return (2.0 - x) * (2.0 - x);
}

int main(int argc, char *argv[]) {
    const double a = -10.0;
    const double b = 10.0;
    long N = 1000000;
if (argc > 1) {
    N = atol(argv[1]);
}

    double global_best_x = 0.0;
    double global_best_f = DBL_MAX;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        unsigned int seed = 1234 + omp_get_thread_num();
        double local_best_x = 0.0;
        double local_best_f = DBL_MAX;

        #pragma omp for
        for (long i = 0; i < N; i++) {
            double r = (double)rand_r(&seed) / RAND_MAX;
            double x = a + (b - a) * r;
            double fx = f(x);

            if (fx < local_best_f) {
                local_best_f = fx;
                local_best_x = x;
            }
        }

        #pragma omp critical
        {
            if (local_best_f < global_best_f) {
                global_best_f = local_best_f;
                global_best_x = local_best_x;
            }
        }
    }

    double end = omp_get_wtime();

    printf("x* = %.10f\n", global_best_x);
    printf("error |x* - 2| = %.10e\n", fabs(global_best_x - 2.0));
    printf("time = %.6f s\n", end - start);

    return 0;
}

