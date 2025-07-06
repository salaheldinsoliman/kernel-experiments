#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <omp.h>

#define N 1500

void *interrupt_simulator(void *arg) {
    while (1) {
        usleep(300000);  // every 0.3 seconds
        volatile double dummy = 0.0;
        for (int i = 0; i < 1e7; i++) dummy += i;  // CPU spike
    }
    return NULL;
}

int main() {
    static double a[N][N], b[N][N], c[N][N];

    pthread_t tid;
    pthread_create(&tid, NULL, interrupt_simulator, NULL);

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

    printf("Interrupt workload completed in %.6f seconds\n", end - start);
    return 0;
}
