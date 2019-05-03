#!/bin/bash

rm "genetic_evals_10.csv"
touch "genetic_evals_10.csv"
../../bin/Slideshow genetic ../../instances/e_shiny_selfies.txt 10 ../../solutions/e.sol -i 999999999 -n 10 -t 120

rm "stoch_evals_1.csv"
touch "stoch_evals_1.csv"
../../bin/Slideshow stoch_descent ../../instances/e_shiny_selfies.txt 10 ../../solutions/e.sol -i 999999999 -n 1 -t 120

rm "annealing_evals.csv"
touch "annealing_evals.csv"
../../bin/Slideshow annealing ../../instances/e_shiny_selfies.txt 10 ../../solutions/e.sol -i 999999999 -n 1 -t 120


python stoch_annealing_genetic.py
