#!/bin/bash

rm greedy_evals.csv
rm greedy_times.csv
touch greedy_evals.csv
touch greedy_times.csv

for i in 2 4 8 16 32 64 128  256  512 1024 2024 4096  8192
do
    START_TIME=$SECONDS
    ../../bin/Slideshow greedy ../../instances/b_lovely_landscapes.txt 100 ../../solutions/b.sol -l $i
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> greedy_times.csv
done

python greedy.py
