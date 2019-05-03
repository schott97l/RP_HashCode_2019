import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import re
import os

if __name__ == "__main__":

    ilp_evals = genfromtxt('ilp_evals.csv', delimiter=',')
    ilp_times = genfromtxt('ilp_times.csv', delimiter=',')

    greedy_evals = genfromtxt('greedy_evals.csv', delimiter=',')
    greedy_times = genfromtxt('greedy_times.csv', delimiter=',')

    x = greedy_times[:,0]

    ilp_eval = ilp_evals
    ilp_time = ilp_times[:,1]

    greedy_eval = greedy_evals[:,1]
    greedy_time = greedy_times[:,1]

    plt.figure()
    plt.xlabel("pourcentage de l'instance",fontsize=16)
    plt.ylabel("Score",fontsize=16)
    #for eval in evals:
    plt.plot(x,ilp_eval,label="PLNE")
    plt.plot(x,greedy_eval,label="Glouton")
    plt.legend(fontsize=16)
    plt.tick_params(labelsize=16)
    plt.show()

    plt.figure()
    plt.xlabel("pourcentage de l'instance",fontsize=16)
    plt.ylabel("Temps (secondes)",fontsize=16)
    #for time in times:
    plt.plot(x,ilp_time,label="PLNE")
    plt.plot(x,greedy_time,label="Glouton")
    plt.legend(fontsize=16)
    plt.tick_params(labelsize=16)
    plt.show()
