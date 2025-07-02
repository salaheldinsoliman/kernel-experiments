// omp_matrix.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 1000  // Increase matrix size for heavier workload

int main(int argc, char *argv[]) {
    int i, j, k, r;
    double **A, **B, **C;

    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    // Allocate matrices
    A = malloc(SIZE * sizeof(double *));
    B = malloc(SIZE * sizeof(double *));
    C = malloc(SIZE * sizeof(double *));
    for (i = 0; i < SIZE; i++) {
        A[i] = malloc(SIZE * sizeof(double));
        B[i] = malloc(SIZE * sizeof(double));
        C[i] = malloc(SIZE * sizeof(double));
    }

    // Initialize A and B
    #pragma omp parallel for private(j)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            A[i][j] = 1.0;
            B[i][j] = 2.0;
        }
    }

    // Matrix multiplication repeated for heavier load
    int repetitions = 2;

    double start = omp_get_wtime();
    for (r = 0; r < repetitions; r++) {
        #pragma omp parallel for private(j, k)
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                C[i][j] = 0.0;
                for (k = 0; k < SIZE; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    double end = omp_get_wtime();

    printf("Done. Elapsed time: %.3f seconds\n", end - start);

    // Clean up
    for (i = 0; i < SIZE; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);

    return 0;
}
