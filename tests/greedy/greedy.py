import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt

if __name__ == "__main__":

    evals = genfromtxt('greedy_evals.csv', delimiter=',')
    times = genfromtxt('greedy_times.csv', delimiter=',')

    x = evals[:,0]
    y1 = evals[:,1]
    y2 = times[:,1]

    fig = plt.figure()
    subplot1 = fig.add_subplot(111)
    subplot2 = subplot1.twinx()
    subplot1.set_xlabel("Nombre de slides à observer",fontsize=16)
    subplot1.set_ylabel("Score",fontsize=16)
    subplot2.set_ylabel("Temps (secondes)",fontsize=16)
    l1, = subplot1.plot(x,y1,'b',label="Score")
    l2, = subplot2.plot(x,y2,'r',label="Temps")
    lns = [l1,l2]
    subplot1.legend(handles=lns, loc="best",fontsize=16)
    subplot1.tick_params(labelsize=16)
    subplot2.tick_params(labelsize=16)
    subplot1.yaxis.label.set_color(l1.get_color())
    subplot2.yaxis.label.set_color(l2.get_color())
    plt.show()
