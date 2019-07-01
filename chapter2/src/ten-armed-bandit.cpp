//######################################################################
// Copyright (C)                                                       #
// 2016-2018 Shangtong Zhang(zhangshangtong.cpp@gmail.com)             #
// 2016 Tian Jun(tianjun.cpp@gmail.com)                                #
// 2016 Artem Oboturov(oboturov@gmail.com)                             #
// 2016 Kenta Shimada(hyperkentakun@gmail.com)                         #
// Permission given to modify the code as long as you keep this        #
// declaration at the top                                              #
//######################################################################
//============================================================================
// Name        : ten-armed-bandit.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <python3.6m\Python.h>
#include <stdexcept>
#include "Bandit.h"
#include "Plot.h"
#include <cmath>

using namespace std;

bool contains(int action, xt::xarray<int> best_action) {
	for (int i=0; i< best_action.size(); i++) {
		if(action == best_action(i)){
			return true;
		}
	}
	return false;
}
retSimulate simulate(int runs, int time, vector<Bandit> bandits) {
	int n = bandits.size();
	xt::xarray<double> best_action_counts = xt::zeros<double>({n, runs, time});
	//for (auto& el : best_action_counts.shape()) {std::cout << el << ", "; }
	xt::xarray<double> rewards = xt::zeros<double>({n, runs, time});
	for(int i=0; i < bandits.size(); i++) {
		std::cout << "Evaluating a bandit problem.." << std::endl;
		for(int r=0; r < runs; r++) {
			bandits[i].reset();
			for(int t=0; t<time;t++) {
				int action = bandits[i].act();
				double reward = bandits[i].step(action);
				rewards(i, r, t) = reward;
				if(contains(action, bandits[i].best_action)) {
					best_action_counts(i, r, t) = 1;
				}
			}
		}
	}
	xt::xarray<double> mean_best_action_counts = xt::mean(best_action_counts, 1);
	xt::xarray<double> mean_rewards = xt::mean(rewards, 1);
	retSimulate retsimulate;
	retsimulate.mean_best_action_counts = mean_best_action_counts;
	retsimulate.mean_rewards = mean_rewards;
	return retsimulate;
}
void figure_2_1(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.1 .." << std::endl;
	plot.figure_2_1();
}

void figure_2_2(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.2 .." << std::endl;
	vector<Bandit> bandits;
	float epsilons[3] = {0, 0.1, 0.01};
	for(int i=0; i<3; i++){
		BanditParams bandit_params;
		bandit_params.epsilon = epsilons[i];
		bandit_params.sample_averages = true;
		Bandit bandit(bandit_params);
		bandits.push_back(bandit);
	}
	retSimulate data = simulate(runs, time, bandits);
	plot.figure_2_2(data, epsilons, 3, time);
 }

void figure_2_3(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.3 .." << std::endl;
	vector<Bandit> bandits;
	float epsilons[2] = {0, 0.1};
	int initials[2] = {5, 0};
	for(int i=0; i<2; i++){
		BanditParams bandit_params;
		bandit_params.epsilon = epsilons[i];
		bandit_params.initial = initials[i];
		Bandit bandit(bandit_params);
		bandits.push_back(bandit);
	}
	retSimulate data = simulate(runs, time, bandits);
	plot.figure_2_3(data, 2, time);
 }

void figure_2_4(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.4 .." << std::endl;
	vector<Bandit> bandits;
	float epsilons[2] = {0, 0.1};
	int ucb_params[2] = {2, -1};
	for(int i=0; i<2; i++){
		BanditParams bandit_params;
		bandit_params.epsilon = epsilons[i];
		bandit_params.UCB_params = ucb_params[i];
		bandit_params.sample_averages = true;
		Bandit bandit(bandit_params);
		bandits.push_back(bandit);
	}
	retSimulate data = simulate(runs, time, bandits);
	plot.figure_2_4(data, 2, time);
 }

void figure_2_5(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.5 .." << std::endl;
	vector<Bandit> bandits;
	float step_sizes[4] = {0.1, 0.1, 0.4, 0.4};
	bool gradient_baselines[4] = {true, false, true, false};
	for(int i=0; i<4; i++){
		BanditParams bandit_params;
		bandit_params.step_size = step_sizes[i];
		bandit_params.gradient_baseline = gradient_baselines[i];
		bandit_params.gradient = true;
		bandit_params.true_rewards = 4;
		Bandit bandit(bandit_params);
		bandits.push_back(bandit);
	}
	retSimulate data = simulate(runs, time, bandits);
	plot.figure_2_5(data, 4, time);
 }

void genProblems(vector<Bandit> *bandits, xt::xarray<double> parameters) {
	int i;
	int j=0;
	for(i=0; i<7; i++) {
		BanditParams bandit_params;
		bandit_params.epsilon = parameters(j, i);
		bandit_params.sample_averages = true;
		Bandit bandit(bandit_params);
		(*bandits).push_back(bandit);
	}
	j=1;
	for(i=0; i<7; i++) {
		BanditParams bandit_params;
		bandit_params.step_size = parameters(j, i);
		bandit_params.gradient = true;
		bandit_params.gradient_baseline = true;
		Bandit bandit(bandit_params);
		(*bandits).push_back(bandit);
	}
	j=2;
	for(i=0; i<7; i++) {
		BanditParams bandit_params;
		bandit_params.epsilon = 0;
		bandit_params.UCB_params = parameters(j, i);
		bandit_params.sample_averages = true;
		Bandit bandit(bandit_params);
		(*bandits).push_back(bandit);
	}
	j=3;
	for(i=0; i<7; i++) {
		BanditParams bandit_params;
		bandit_params.epsilon = 0;
		bandit_params.initial = parameters(j, i);
		bandit_params.step_size = 0.1;
		Bandit bandit(bandit_params);
		(*bandits).push_back(bandit);
	}
}

void genParams(xt::xarray<double> * parameters) {
	// -8, -1
	xt::xarray<double> a1 = xt::arange<double>(-8, -1);
	int j = 0;
	for (auto& el : a1) {
		(*parameters)(0, j) = std::pow(2, (float)el);
		j++;
	}
	// -5, 2
	xt::xarray<double> a2 = xt::arange<double>(-5, 2);
	j = 0;
	for (auto& el : a2) {
		(*parameters)(1, j) = std::pow(2, (float)el);
		j++;
	}
	// -4, 3
	xt::xarray<double> a3 = xt::arange<double>(-4, 3);
	j = 0;
	for (auto& el : a3) {
		(*parameters)(2, j) = std::pow(2, (float)el);
		j++;
	}
	// -4, 3
	xt::xarray<double> a4 = xt::arange<double>(-4, 3);
	j = 0;
	for (auto& el : a4) {
		(*parameters)(3, j) = std::pow(2, (float)el);
		j++;
	}
}

void figure_2_6(Plot& plot, int runs=2000, int time=1000) {
	std::cout << "Figure 2.6 .." << std::endl;
	vector<Bandit> bandits;
	xt::xarray<double> parameters = xt::zeros<double>({4, 7});
	genParams(&parameters);
	genProblems(&bandits, parameters);
	// This returns a 2D array: 28 x 1000. Each row is a bandit problem
	// Columns represent time steps. Each element is the average reward of bandit problem
	// x at time y
	retSimulate data = simulate(runs, time, bandits);
	// rewards is a 1 D array of size 28. Each element is the mean reward across all time steps
	// of a bandit problem. [0: 7) epsilon greedy problems
	// [7, 14) gradient bandit
	// [14, 21) UCB
	// [21, 28) optimistic initialization
	xt::xarray<double> rewards  = xt::mean(data.mean_rewards, 1);
	plot.figure_2_6(rewards, parameters, rewards.size());
	/*float step_sizes[4] = {0.1, 0.1, 0.4, 0.4};  4-7
	bool gradient_baselines[4] = {true, false, true, false};
	for(int i=0; i<4; i++){
		BanditParams bandit_params;
		bandit_params.step_size = step_sizes[i];
		bandit_params.gradient_baseline = gradient_baselines[i];
		bandit_params.gradient = true;
		bandit_params.true_rewards = 4;
		Bandit bandit(bandit_params);
		bandits.push_back(bandit);
	}
	retSimulate data = simulate(runs, time, bandits);
	plot.figure_2_5(data, 4, time); */
 }


int main()
{
	xt::xarray<double> a = xt::random::randn<double>({10});
	BanditParams bandit_params;
	Plot plot;
	figure_2_1(plot);
	figure_2_2(plot);
	figure_2_3(plot);
	figure_2_4(plot);
	figure_2_5(plot);
	figure_2_6(plot);
	return 0;
}
