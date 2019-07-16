import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
matplotlib.use('Agg')
GOAL = 100
STATES = np.arange(GOAL + 1)

def plot_figure(sweeps_history=[], policy=[]):
    plt.figure(figsize=(10, 20))

    plt.subplot(2, 1, 1)
    for sweep, state_value in enumerate(sweeps_history):
        plt.plot(state_value, label='sweep {}'.format(sweep))
    plt.xlabel('Capital')
    plt.ylabel('Value estimates')
    plt.legend(loc='best')

    plt.subplot(2, 1, 2)
    plt.scatter(STATES, policy)
    plt.xlabel('Capital')
    plt.ylabel('Final policy (stake)')
    plt.savefig(os.environ['WORKING_DIR'] + '/figure_4_3.png')
    plt.close()