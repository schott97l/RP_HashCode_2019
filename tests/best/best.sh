#!/bin/bash

rm stoch_evals_1.csv
touch stoch_evals_1.csv
../../bin/Slideshow naive_greedy ../../instances/b_lovely_landscapes.txt 100 ../../solutions/b.sol
../../bin/Slideshow stoch_descent ../../instances/b_lovely_landscapes.txt 100 ../../solutions/b.sol -s ../../solutions/b.sol -n 1 -i 99999999999 -t 300
mv stoch_evals_1.csv b.csv

rm annealing_evals.csv
touch annealing_evals.csv
../../bin/Slideshow greedy ../../instances/c_memorable_moments.txt 100 ../../solutions/c.sol -l 1000
../../bin/Slideshow annealing ../../instances/c_memorable_moments.txt 100 ../../solutions/c.sol -s ../../solutions/c.sol -i 99999999999 -t 300
mv annealing_evals.csv c.csv

rm annealing_evals.csv
touch annealing_evals.csv
../../bin/Slideshow greedy ../../instances/d_pet_pictures.txt 100 ../../solutions/d.sol -l 1000
../../bin/Slideshow annealing ../../instances/d_pet_pictures.txt 100 ../../solutions/d.sol -s ../../solutions/d.sol -i 99999999999 -t 300
mv annealing_evals.csv d.csv

rm annealing_evals.csv
touch annealing_evals.csv
../../bin/Slideshow greedy ../../instances/e_shiny_selfies.txt 100 ../../solutions/e.sol -l 1000
../../bin/Slideshow annealing ../../instances/e_shiny_selfies.txt 100 ../../solutions/e.sol -s ../../solutions/e.sol -i 99999999999 -t 300
mv annealing_evals.csv e.csv
