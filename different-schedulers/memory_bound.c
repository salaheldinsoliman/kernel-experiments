// memory_bound.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE (1L<<30)  // 1GB array

int main() {
    char *arr = malloc(SIZE);
    if (!arr) { perror("malloc"); return 1; }
    clock_t start = clock();
    for (long i = 0; i < SIZE; i += 64)  // stride to touch every cache line
        arr[i] += 1;
    clock_t end = clock();
    printf("Memory walk done.\n");
    printf("Elapsed time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    free(arr);
    return 0;
}
