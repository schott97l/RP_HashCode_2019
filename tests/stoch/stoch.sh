#!/bin/bash

for i in 1 5 10 50 100
do
    rm "stoch_evals_$i.csv"
    touch "stoch_evals_$i.csv"
    ../../bin/Slideshow stoch_descent ../../instances/d_pet_pictures.txt 1 ../../solutions/d.sol -i 999999999 -n $i -t 10
done

python stoch.py
