#!/bin/bash

set -e

# ----------------------
# Compilation
# ----------------------
echo "[INFO] Compiling workloads..."

gcc -O3 -fopenmp compute_stable.c -o compute_stable
gcc -O3 -fopenmp -lpthread compute_interrupt.c -o compute_interrupt

echo "[INFO] Compilation complete."

# ----------------------
# Detect preemption mode from kernel config
# ----------------------
echo "[INFO] Detecting preemption mode..."

PREEMPT_MODE="unknown"

if zgrep -q "^CONFIG_PREEMPT_NONE=y" /proc/config.gz 2>/dev/null; then
    PREEMPT_MODE="preempt_none"
elif zgrep -q "^CONFIG_PREEMPT_VOLUNTARY=y" /proc/config.gz 2>/dev/null; then
    PREEMPT_MODE="preempt_voluntary"
elif zgrep -q "^CONFIG_PREEMPT=y" /proc/config.gz 2>/dev/null; then
    PREEMPT_MODE="preempt"
else
    echo "[WARNING] Could not determine preemption mode from /proc/config.gz; trying /boot..."
    if grep -q "^CONFIG_PREEMPT_NONE=y" /boot/config-$(uname -r); then
        PREEMPT_MODE="preempt_none"
    elif grep -q "^CONFIG_PREEMPT_VOLUNTARY=y" /boot/config-$(uname -r); then
        PREEMPT_MODE="preempt_voluntary"
    elif grep -q "^CONFIG_PREEMPT=y" /boot/config-$(uname -r); then
        PREEMPT_MODE="preempt"
    fi
fi

echo "[INFO] Detected preemption mode: $PREEMPT_MODE"

# ----------------------
# Results file
# ----------------------
RESULTS="results_${PREEMPT_MODE}.txt"
echo "[INFO] Results will be stored in: $RESULTS"
echo "Experiment started at: $(date)" > "$RESULTS"

# ----------------------
# Single stable workload run
# ----------------------
echo "[INFO] Running stable workload..."
echo "=== Stable Run ===" >> "$RESULTS"

taskset -c 2 ./compute_stable >> "$RESULTS" 2>&1

echo "[INFO] Stable workload complete."

# ----------------------
# Single interrupt workload run
# ----------------------
echo "[INFO] Running interrupt workload..."
echo "=== Interrupt Run ===" >> "$RESULTS"

taskset -c 2 ./compute_interrupt >> "$RESULTS" 2>&1

echo "[INFO] Interrupt workload complete."

echo "[INFO] Experiment complete!"
