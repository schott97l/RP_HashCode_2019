#!/bin/bash

rm greedy_times.csv
touch greedy_times.csv
rm greedy_evals.csv
touch greedy_evals.csv
for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
do
    START_TIME=$SECONDS
    ../../bin/Slideshow greedy ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -l 1000
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> greedy_times.csv
done

#rm ilp_evals.csv
#touch ilp_evals.csv
#rm ilp_times.csv
#touch ilp_times.csv
#for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
for i in 0.7 0.8
do
    START_TIME=$SECONDS
    ../../bin/Slideshow ilp ../../instances/b_lovely_landscapes.txt $i ../../solutions/b.sol -f "flow"
    ELAPSED_TIME=$(($SECONDS - $START_TIME))
    echo $i "," $ELAPSED_TIME >> ilp_times.csv
done

python ilp.py
