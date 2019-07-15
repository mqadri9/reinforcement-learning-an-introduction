import sys, os
sys.path.insert(0, os.environ['WORKING_DIR'])
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
_, axes = plt.subplots(2, 3, figsize=(40, 20))
plt.subplots_adjust(wspace=0.1, hspace=0.2)
axes = axes.flatten();
matplotlib.use('Agg')

def plot_figure(policies, iterations, value):
    for i in range(len(policies)):
        p = np.flipud(policies[i])
        a = int(iterations[i])
        fig = sns.heatmap(np.flipud(p), cmap="YlGnBu", ax=axes[a])
        fig.set_ylabel('# cars at first location', fontsize=30)
        fig.set_yticks(list(reversed(range(21))))
        fig.set_xlabel('# cars at second location', fontsize=30)
        fig.set_title('policy {}'.format(a), fontsize=30)
    fig = sns.heatmap(np.flipud(value), cmap="YlGnBu", ax=axes[-1])
    fig.set_ylabel('# cars at first location', fontsize=30)
    fig.set_yticks(list(reversed(range(21))))
    fig.set_xlabel('# cars at second location', fontsize=30)
    fig.set_title('optimal value', fontsize=30)
    plt.savefig(os.environ['WORKING_DIR'] + '/figure_4_2.png')
    plt.close()