#!/bin/bash

for i in 10 20 40 80
do
    rm "genetic_evals_$i.csv"
    touch "genetic_evals_$i.csv"
    ../../bin/Slideshow genetic ../../instances/d_pet_pictures.txt 0.1 ../../solutions/d.sol -i 999999999 -n $i -t 120
done

python genetic.py
