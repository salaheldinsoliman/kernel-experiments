// compute_bound.c
#include <stdio.h>
#include <time.h>

#define N 100000

int main() {
    double sum = 0.0;
    clock_t start = clock();
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            sum += i * 0.000001 + j * 0.000002;
    clock_t end = clock();
    printf("Compute sum: %f\n", sum);
    printf("Elapsed time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
