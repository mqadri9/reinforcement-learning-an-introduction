import matplotlib
import os
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def plot_figure(pythonEstimateValues):
    for est in pythonEstimateValues:
        plt.plot(est)
    plt.xlabel('Episodes (log scale)')
    plt.ylabel('Ordinary Importance Sampling')
    plt.ylim((0,3))
    plt.savefig(os.environ['WORKING_DIR'] + '/figure_5_4.png')
    plt.close()
    
    