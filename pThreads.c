#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define M 1000
#define N 1000
#define K 1000
#define NUM_THREADS 4

double A[M][K];
double B[K][N];
double C[M][N];
int step_i = 0;

void* multiply(void* arg) {
    int core = step_i++;
    for (int i = core * M / NUM_THREADS; i < (core + 1) * M / NUM_THREADS; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

int main() {
    // Initialize matrices
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            A[i][j] = rand() % 100;
        }
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 100;
        }
    }

    // Perform matrix multiplication sequentially
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Time for sequential: %lf seconds\n", time_spent);

    // Perform matrix multiplication in parallel
    pthread_t threads[NUM_THREADS];
    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, multiply, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Time for Pthreads: %lf seconds\n", time_spent);

    return 0;
}
