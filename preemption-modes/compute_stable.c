#include <stdio.h>
#include <omp.h>

#define N 2000

int main() {
    static double a[N][N], b[N][N], c[N][N];

    double start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            a[i][j] = i + j;
            b[i][j] = i - j;
            c[i][j] = 0.0;
        }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                c[i][j] += a[i][k] * b[k][j];

    double end = omp_get_wtime();

    printf("Stable workload completed in %.6f seconds\n", end - start);
    return 0;
}
