#!/bin/bash

# Directory containing the performance results
PERF_DIR="scalability"

# Output file for summary
SUMMARY_FILE="perf_summary.csv"

# Initialize CSV file with headers
echo "Threads,Experiment,Elapsed(s)" > $SUMMARY_FILE

# Loop through thread directories (e.g., 1-threads, 2-threads, etc.)
for thread_dir in "$PERF_DIR"/*-threads; do
    # Extract thread count from directory name
    THREADS=$(basename "$thread_dir" | grep -oP '^\d+')

    # Loop through experiment directories (elephant, globe, pianoroom)
    for experiment in "$thread_dir"/*; do
        EXPERIMENT_NAME=$(basename "$experiment")
        
        # Path to perfstat.txt
        PERFSTAT_FILE="$experiment/perfstat.txt"
        
        if [[ -f "$PERFSTAT_FILE" ]]; then
            # Extract task-clock value
            TASK_CLOCK=$(grep "time elapsed" "$PERFSTAT_FILE" | awk '{print $1}')
            
            # Append to summary file
            echo "$THREADS,$EXPERIMENT_NAME,$TASK_CLOCK" >> $SUMMARY_FILE
        fi
    done
done

# Display results
echo "Summary written to $SUMMARY_FILE"
cat $SUMMARY_FILE
