import matplotlib.pyplot as plt
import os

def plot_figure_2_2(rewards, best_action_counts, epsilons):
    plt.figure(figsize=(10, 20))
    plt.subplot(2, 1, 1)
    for eps, rewards in zip(epsilons, rewards):
        plt.plot(rewards, label='epsilon = %.02f' % (eps))
    plt.xlabel('steps')
    plt.ylabel('average reward')
    plt.legend()
    plt.subplot(2, 1, 2)
    for eps, counts in zip(epsilons, best_action_counts):
        plt.plot(counts, label='epsilon = %.02f' % (eps))
    plt.xlabel('steps')
    plt.ylabel('% optimal action')
    plt.legend()
    plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_2.png')
    plt.close()
    
    
def plot_figure_2_3(best_action_counts):
    plt.plot(best_action_counts[0], label='epsilon = 0, q = 5')
    plt.plot(best_action_counts[1], label='epsilon = 0.1, q = 0')
    plt.xlabel('Steps')
    plt.ylabel('% optimal action')
    plt.legend()
    plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_3.png')
    plt.close()
    
    
def plot_figure_2_4(average_rewards):
    plt.plot(average_rewards[0], label='UCB c = 2')
    plt.plot(average_rewards[1], label='epsilon greedy epsilon = 0.1')
    plt.xlabel('Steps')
    plt.ylabel('Average reward')
    plt.legend()
    plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_4.png')
    plt.close()


def plot_figure_2_5(best_action_counts):
    labels = ['alpha = 0.1, with baseline',
              'alpha = 0.1, without baseline',
              'alpha = 0.4, with baseline',
              'alpha = 0.4, without baseline']

    for i in range(4):
        plt.plot(best_action_counts[i], label=labels[i])
    plt.xlabel('Steps')
    plt.ylabel('% Optimal action')
    plt.legend()
    plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_5.png')
    plt.close()
    
def plot_figure_2_6(rewards, parameters):
    labels = ['epsilon-greedy', 'gradient bandit',
              'UCB', 'optimistic initialization']
    i = 0
    for label, parameter in zip(labels, parameters):
        l = len(parameter)
        plt.plot(parameter, rewards[i:i+l], label=label)
        i += l
    plt.xlabel('Parameter(2^x)')
    plt.ylabel('Average reward')
    plt.legend()
    plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_6.png')
    plt.close()  