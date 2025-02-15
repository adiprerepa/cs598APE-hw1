#!/bin/bash

# Define the number of threads to test
THREAD_COUNTS=(16)

# Ensure the scalability directory exists
mkdir -p scalability

for i in "${THREAD_COUNTS[@]}"; do
    # export OMP_NUM_THREADS=$i
    export PERF_OUT_LOCATION="scalability/${i}-threads"
    
    echo "Running with OMP_NUM_THREADS=$OMP_NUM_THREADS, results in $PERF_OUT_LOCATION"
    
    bash perfreport.sh -PEG
done
