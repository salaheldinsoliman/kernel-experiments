# Scheduler Policy Experiment for HPC Workloads

This experiment explores how different **Linux scheduling policies** affect the performance of various HPC workload types under the default **Completely Fair Scheduler (CFS)**.

We evaluate:
- Compute-bound
- Memory-bound
- I/O-bound
- Mixed compute + I/O workloads

Each workload runs under three scheduling policies:
- **SCHED_OTHER**: default CFS fair scheduling.
- **SCHED_FIFO**: fixed-priority real-time scheduling.
- **SCHED_RR**: round-robin real-time scheduling.

The goal is to determine which scheduling policy yields the lowest runtime for each workload type.

---

## 🔹 Workloads

- **compute_bound.c**: performs large matrix-like floating-point calculations.
- **memory_bound.c**: walks a large array to stress memory bandwidth.
- **io_bound.c**: writes a large file to disk, simulating I/O-heavy workloads.
- **mixed.c**: combines compute-intensive work with periodic disk checkpointing.

---

## 🔹 Script

The script `run_hpc_experiments.sh`:
- Compiles the benchmarks.
- Runs each workload under all three schedulers.
- Records elapsed times.
- Sorts schedulers for each workload from fastest to slowest.
- Outputs results to `experiment_results.txt`.

---

## 🔹 Running the Experiment

1. Make the script executable:
   ```bash
   chmod +x run_hpc_experiments.sh
   ```

2. View the results:
    ```bash
    cat experiment_results.txt
    ```
