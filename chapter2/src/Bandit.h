/*
 * Bandit.h
 *
 *  Created on: Jun 9, 2019
 *      Author: moham
 */

#ifndef BANDIT_H_
#define BANDIT_H_
#include <stdio.h>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include <complex>
#include "xtensor/xcomplex.hpp"
#include "xtensor/xbuilder.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xsort.hpp"
#include "xtensor/xfixed.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include "xtensor/xadapt.hpp"

// @k_arm: # of arms
// @epsilon: probability for exploration in epsilon-greedy algorithm
// @initial: initial estimation for each action
// @step_size: constant step size for updating estimations
// @sample_averages: if True, use sample averages to update estimations instead of constant step size
// @UCB_param: if not None, use UCB algorithm to select action
// @gradient: if True, use gradient based bandit algorithm
// @gradient_baseline: if True, use average reward as baseline for gradient based bandit algorithm
// @xtime: number of steps

struct BanditParams
{
	int k_arm = 10;
	float epsilon = 0.0;
	float initial = 0.0;
	float step_size = 0.1;
	bool sample_averages = false;
	int UCB_params = -1;
	bool gradient = false;
	bool gradient_baseline = false;
	float true_rewards = 0.0;
};

struct retSimulate {
	xt::xarray<double> mean_best_action_counts;
	xt::xarray<double> mean_rewards;
};

class Bandit {
public:
	BanditParams bandit_params;
	xt::xarray<double> q_true;
	xt::xarray<double> q_estimation;
	xt::xarray<int> action_count;
	xt::xarray<int> best_action;
	xt::xarray<double> action_prob;
	int xtime = 0;
	float average_reward = 0;
	xt::xarray<int> indices;

public:
	Bandit(BanditParams bandit_params);
	virtual ~Bandit();
	void reset();
	int act();
	int act_non_greedy();
	int act_greedy();
	int act_ucb();
	int act_gradient();
	int findMax(xt::xarray<double> a);
	float step(int action);
};

#endif /* BANDIT_H_ */
