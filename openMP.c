#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    int i, j, k;
    int m = 5000, n = 5000, p = 5000;
    double *A, *B, *C;
    double t1, t2;

    // Allocate memory for matrices
    A = (double*)malloc(m * n * sizeof(double));
    B = (double*)malloc(n * p * sizeof(double));
    C = (double*)malloc(m * p * sizeof(double));

    // Initialize matrices A and B
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            A[i*n+j] = i+j;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            B[i*p+j] = i+j;
        }
    }

    t1 = omp_get_wtime(); // Start timer

    #pragma omp parallel for private(j,k) shared(A,B,C) schedule(dynamic)
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            double sum = 0.0;
            for (k = 0; k < n; k++) {
                sum += A[i*n+k] * B[k*p+j];
            }
            C[i*p+j] = sum;
        }
    }

    t2 = omp_get_wtime(); // Stop timer

    printf("Time for OpenMP: %f seconds\n", t2-t1);

    // Free memory
    free(A);
    free(B);
    free(C);

    return 0;
}
