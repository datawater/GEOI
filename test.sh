#! /usr/bin/env bash

if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <executable> <directory> <memory_limit_bytes> <time_limit_ms>"
    exit 1
fi

executable="$1"
directory="$2"
memory_limit="$3"
time_limit="$4"

if [ ! -x "$executable" ]; then
    echo "Error: '$executable' is not an executable file."
    exit 1
fi

if [ ! -d "$directory" ]; then
    echo "Error: '$directory' is not a directory."
    exit 1
fi

for input_file in "$directory"/*.in; do
    if [[ "$input_file" =~ (.*)\.in$ ]]; then
        base_name="${BASH_REMATCH[1]}"
        sol_file="$base_name.sol"

        echo Testing $base_name >&2

        if [ -f "$sol_file" ]; then            
            output=$( /usr/bin/env time -f "%M %e" -o temp_usage.txt timeout $((time_limit / 1000))s "$executable" < "$input_file" 2>&1 )
            exit_code=$?
            
            if [ "$exit_code" -ne 0 ]; then
                echo "Test failed for input: $input_file (timeout or execution error)"
                exit 1
            fi
            
            expected_output=$(cat "$sol_file")
            memory_used=$(awk '{print $1}' temp_usage.txt)
            time_used=$(awk '{print $2 * 1000}' temp_usage.txt)
            
            if [ "$memory_used" -gt "$memory_limit" ]; then
                echo "Test failed for input: $input_file (memory limit exceeded)"
                exit 1
            fi

            if (( $(echo "$time_used > $time_limit" | bc -l) )); then
                echo "Test failed for input: $input_file (time limit exceeded)"
                exit 1
            fi
            
            if [ "$output" != "$expected_output" ]; then
                echo "Test failed for input: $input_file (wrong output)"
                exit 1
            fi
        fi
    fi
done

echo "All tests passed successfully."
