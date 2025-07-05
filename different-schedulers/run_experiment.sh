#!/bin/bash

OUTPUT="experiment_results.txt"
> "$OUTPUT"  # clear output file

PROGRAMS=("compute_bound" "memory_bound" "io_bound" "mixed")
SCHEDULERS=("SCHED_OTHER" "SCHED_FIFO" "SCHED_RR")

# Step 1: Compile programs
echo "Compiling programs..."
gcc -O2 -o compute_bound compute_bound.c
gcc -O2 -o memory_bound memory_bound.c
gcc -O2 -o io_bound io_bound.c
gcc -O2 -o mixed mixed.c

# Step 2: Run experiments
for prog in "${PROGRAMS[@]}"; do
  echo -e "\n=== $prog ===" | tee -a "$OUTPUT"
  declare -A times  # associative array: times[Sched]="Time"

  for sched in "${SCHEDULERS[@]}"; do
    echo -n "Running $prog with $sched... "
    case $sched in
      SCHED_OTHER)
        elapsed=$( { /usr/bin/time -f "%e" ./$prog > /dev/null; } 2>&1 )
        ;;
      SCHED_FIFO)
        elapsed=$( { sudo /usr/bin/time -f "%e" chrt -f 99 ./$prog > /dev/null; } 2>&1 )
        ;;
      SCHED_RR)
        elapsed=$( { sudo /usr/bin/time -f "%e" chrt -r 99 ./$prog > /dev/null; } 2>&1 )
        ;;
    esac
    echo "Elapsed time: $elapsed seconds" | tee -a "$OUTPUT"
    times[$sched]=$elapsed
  done

  # sort scheduler-time pairs
  echo ">> Scheduler ranking for $prog:" | tee -a "$OUTPUT"
  for sched in "${!times[@]}"; do
    echo "$sched ${times[$sched]}"
  done | sort -k2 -n | tee -a "$OUTPUT"

  unset times
done

echo -e "\nAll experiments completed. See $OUTPUT for details."
