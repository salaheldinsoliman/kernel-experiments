// io_bound.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_SIZE (1L<<29)  // 512MB
#define FILENAME "io_testfile.dat"

int main() {
    char *buffer = malloc(4096);
    if (!buffer) { perror("malloc"); return 1; }
    memset(buffer, 'A', 4096);

    FILE *fp = fopen(FILENAME, "w");
    if (!fp) { perror("fopen"); return 1; }

    clock_t start = clock();
    for (long i = 0; i < FILE_SIZE; i += 4096) {
        fwrite(buffer, 1, 4096, fp);
    }
    fclose(fp);
    clock_t end = clock();
    printf("I/O write done.\n");
    printf("Elapsed time: %.2f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    free(buffer);
    return 0;
}
