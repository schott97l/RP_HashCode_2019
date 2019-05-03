#!/bin/bash

rm round_times.csv
touch round_times.csv
rm round_evals.csv
touch round_evals.csv

for i in 0.25 0.5 0.75 1 1.25 1.5 1.75 2 2.25
do
    START_TIME=$SECONDS
    ../../bin/Slideshow round ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -f ""
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> round_times.csv
done

rm greedy_times.csv
touch greedy_times.csv
rm greedy_evals.csv
touch greedy_evals.csv

for i in 0.25 0.5 0.75 1 1.25 1.5 1.75 2 2.25
do
    START_TIME=$SECONDS
    ../../bin/Slideshow greedy ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -f ""
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> greedy_times.csv
done



python round_greedy.py
