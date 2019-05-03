import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import re
import os

if __name__ == "__main__":

    round_evals = genfromtxt('round_evals.csv', delimiter=',')
    round_times = genfromtxt('round_times.csv', delimiter=',')
    greedy_evals = genfromtxt('greedy_evals.csv', delimiter=',')
    greedy_times = genfromtxt('greedy_times.csv', delimiter=',')
    ilp_evals = genfromtxt('ilp_evals.csv', delimiter=',')
    ilp_times = genfromtxt('ilp_times.csv', delimiter=',')

    x = round_times[:,0]
    yr1 = round_evals[:]
    yr2 = round_times[:,1]
    yg1 = greedy_evals[:,1]
    yg2 = greedy_times[:,1]
    yi1 = ilp_evals[:]
    yi2 = ilp_times[:,1]


    fig = plt.figure()
    subplot = fig.add_subplot(111)
    subplot.set_xlabel("pourcentage de l'instance",fontsize=16)
    subplot.set_ylabel("Score",fontsize=16)
    subplot.plot(x,yr1,'r',label="méthode d'arrondi")
    subplot.plot(x,yg1,'b',label="méthode gloutone")
    subplot.plot(x,yi1,'g',label="PLNE")
    subplot.legend(fontsize=16)
    subplot.tick_params(labelsize=16)
    plt.show()

    fig = plt.figure()
    subplot = fig.add_subplot(111)
    subplot.set_xlabel("pourcentage de l'instance",fontsize=16)
    subplot.set_ylabel("Temps (secondes)",fontsize=16)
    subplot.plot(x,yr2,'r',label="méthode d'arrondi")
    subplot.plot(x,yg2,'b',label="méthode glourone")
    subplot.plot(x,yi2,'g',label="PLNE")
    subplot.legend(fontsize=16)
    subplot.tick_params(labelsize=16)
    plt.show()
