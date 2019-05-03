import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import re
import os

if __name__ == "__main__":

    for file in os.listdir("."):
        regex = re.search(r'^genetic_evals_([0-9]*).csv$',file)
        if regex:
            genetic = genfromtxt(file, delimiter=',')
        regex = re.search(r'^stoch_evals_([0-9]*).csv$',file)
        if regex:
            stoch = genfromtxt(file, delimiter=',')
        regex = re.search(r'^annealing_evals.csv$',file)
        if regex:
            annealing = genfromtxt(file, delimiter=',')


    x = genetic[:,0]

    plt.figure()
    plt.xlabel("Temps (secondes)",fontsize=16)
    plt.ylabel("Score",fontsize=16)
    plt.plot(x,genetic[:,1],label="algorithme génétique")
    plt.plot(x,stoch[:,1],label="descente stochastique")
    plt.plot(x,annealing[:,1],label="recuit simulé")
    plt.tick_params(labelsize=16)
    plt.legend(fontsize=16)
    plt.show()
