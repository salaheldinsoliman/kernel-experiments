# Measuring CPU Isolation and Tickless Kernel Performance

## Introduction

This experiment measures the performance impact of isolating CPUs and enabling full tickless mode (nohz_full) on Linux. Specifically, it benchmarks a simple OpenMP-based matrix multiplication program pinned to isolated cores. By comparing runtimes before and after applying kernel isolation and tickless settings, the experiment evaluates effects on mean execution time and jitter.

The motivation is to understand how CPU isolation and tickless kernel configurations affect deterministic performance for high-performance or real-time workloads.

## Setup

### Virtual Machine

Use VirtualBox to create a VM.

Install Linux Mint (21.x or later recommended).

Allocate 5 virtual CPU cores to the VM.

### omp_matrix example

This is a simple C program performing matrix multiplication using OpenMP with configurable matrix size.

omp_matrix.c:
``` c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define SIZE 1024

int main(int argc, char *argv[]) {
    int threads = 2;
    if (argc > 1) threads = atoi(argv[1]);

    double **A = malloc(SIZE * sizeof(double *));
    double **B = malloc(SIZE * sizeof(double *));
    double **C = malloc(SIZE * sizeof(double *));
    for (int i = 0; i < SIZE; i++) {
        A[i] = malloc(SIZE * sizeof(double));
        B[i] = malloc(SIZE * sizeof(double));
        C[i] = malloc(SIZE * sizeof(double));
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0.0;
        }
    }

    double start = omp_get_wtime();
    omp_set_num_threads(threads);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                C[i][j] += A[i][k] * B[k][j];

    double end = omp_get_wtime();
    printf("%f\n", end - start);
    return 0;
}
```

To compile:

`gcc -O2 -fopenmp omp_matrix.c -o omp_matrix`

### Compiling the Linux kernel

Download and extract the Linux kernel source (e.g., 6.8.x) from kernel.org.

Install build dependencies:

`sudo apt install build-essential libncurses-dev bison flex libssl-dev libelf-dev`

Configure the kernel:

```
cd linux-6.8
make menuconfig
```

the set: 
`CONFIG_NO_HZ_FULL=y`

Build and install the kernel:

```
make -j$(nproc)
sudo make modules_install
sudo make install
```

### Editing GRUB boot options

Edit `/etc/default/grub`:

`sudo nano /etc/default/grub`

Add or update the following kernel command line:

`GRUB_CMDLINE_LINUX="quiet splash isolcpus=1,2 nohz_full=1,2`

Save and update GRUB:

`sudo update-grub`

Reboot and select your compiled kernel (e.g., 6.8.0) from the GRUB boot menu.

Verify boot parameters:

`cat /proc/cmdline`

Check running kernel:

`uname -r`

## Running the experiment

Benchmark script

We sse a script like this to run multiple measurements pinned to isolated CPUs and calculate mean + jitter:

Run it with:
``` bash
chmod +x run_experiment.sh
./run_experiment.sh with-kernel-mod-result.txt
```

This runs the matrix calculations, pinned to CPUs 1&2

