#!/bin/bash

rm round_times.csv
touch round_times.csv
rm round_evals.csv
touch round_evals.csv

for i in 0.5 0.55 0.6 0.65 0.7 0.75
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

for i in 0.5 0.55 0.6 0.65 0.7 0.75 0.8
do
    START_TIME=$SECONDS
    ../../bin/Slideshow greedy ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -l 1000
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> greedy_times.csv
done

rm ilp_times.csv
touch ilp_times.csv
rm ilp_evals.csv
touch ilp_evals.csv

for i in 0.5 0.55 0.6 0.65 0.7 0.75 0.8
do
    START_TIME=$SECONDS
    ../../bin/Slideshow ilp ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -f "flow"
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> ilp_times.csv
done




python round_greedy_ilp.py
