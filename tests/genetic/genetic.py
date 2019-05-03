import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import re
import os

if __name__ == "__main__":

    evals = []
    for file in os.listdir("."):
        regex = re.search(r'^genetic_evals_([0-9]*).csv$',file)
        if regex:
            evals.append((int(regex.group(1)),genfromtxt(file, delimiter=',')))

    evals = sorted(evals,key=lambda tup: tup[0])

    x = evals[0][1][:,0]

    plt.figure()
    plt.xlabel("Temps (secondes)", fontsize=16)
    plt.ylabel("Score", fontsize=16)
    for eval in evals:
        plt.plot(x,eval[1][:,1],label="population : {}".format(eval[0]))
    plt.tick_params(labelsize=16)
    plt.legend(fontsize=16)
    plt.show()
    plt.savefig("result.png")
