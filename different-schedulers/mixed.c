// mixed.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 15000
#define FILE_SIZE (1L<<27)  // 128MB checkpoint file
#define FILENAME "mixed_checkpoint.dat"

int main() {
    double sum = 0.0;
    char *buffer = malloc(4096);
    if (!buffer) { perror("malloc"); return 1; }
    memset(buffer, 'B', 4096);

    FILE *fp = fopen(FILENAME, "w");
    if (!fp) { perror("fopen"); return 1; }

    clock_t start = clock();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            sum += i * 0.000001 + j * 0.000002;
        if (i % 1000 == 0)  // checkpoint every 1000 iterations
            fwrite(buffer, 1, 4096, fp);
    }
    fclose(fp);
    clock_t end = clock();
    printf("Mixed workload sum: %f\n", sum);
    printf("Elapsed time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    free(buffer);
    return 0;
}
