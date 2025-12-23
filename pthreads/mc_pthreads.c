#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>
#include <math.h>

typedef struct {
    long samples;
    double a;
    double b;
    unsigned int seed;
} thread_data_t;

double global_best_x = 0.0;
double global_best_f = DBL_MAX;
pthread_mutex_t mutex;

double f(double x) {
    return (2.0 - x) * (2.0 - x);
}

void* worker(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;

    double local_best_x = 0.0;
    double local_best_f = DBL_MAX;
    unsigned int seed = data->seed;

    for (long i = 0; i < data->samples; i++) {
        double r = (double)rand_r(&seed) / RAND_MAX;
        double x = data->a + (data->b - data->a) * r;
        double fx = f(x);

        if (fx < local_best_f) {
            local_best_f = fx;
            local_best_x = x;
        }
    }

    pthread_mutex_lock(&mutex);
    if (local_best_f < global_best_f) {
        global_best_f = local_best_f;
        global_best_x = local_best_x;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    int num_threads = 4;
if (argc > 2) {
    num_threads = atoi(argv[2]);
}

    long N = 1000000;
if (argc > 1) {
    N = atol(argv[1]);
}
    const double a = -10.0;
    const double b = 10.0;

    pthread_t threads[num_threads];
    thread_data_t data[num_threads];

    pthread_mutex_init(&mutex, NULL);

    long samples_per_thread = N / num_threads;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        data[i].samples = samples_per_thread;
        data[i].a = a;
        data[i].b = b;
        data[i].seed = 1234 + i;

        pthread_create(&threads[i], NULL, worker, &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) * 1e-9;

    printf("x* = %.10f\n", global_best_x);
    printf("error |x* - 2| = %.10e\n", fabs(global_best_x - 2.0));
    printf("time = %.6f s\n", time);

    pthread_mutex_destroy(&mutex);
    return 0;
}

