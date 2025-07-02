#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <output_filename>"
    exit 1
fi

OUTPUT="$1"
RUNS=20
PROGRAM="taskset -c 1,2 ./omp_matrix 2"

echo "Running $PROGRAM for $RUNS runs..."
echo -n "" > "$OUTPUT"

for i in $(seq 1 $RUNS); do
    echo -n "Run $i: "
    elapsed=$($PROGRAM | grep "Elapsed time" | awk '{print $4}')
    echo "$elapsed" | tee -a "$OUTPUT"
done

LC_NUMERIC=C awk -v output="$OUTPUT" '
{
  n++;
  sum += $1;
  sumsq += ($1)^2;
}
END {
  mean = sum/n;
  stddev = sqrt(sumsq/n - mean^2);
  printf "Mean time: %.4f seconds\n", mean | "tee -a " output;
  printf "Std deviation (jitter): %.4f seconds\n", stddev | "tee -a " output;
}' "$OUTPUT"
