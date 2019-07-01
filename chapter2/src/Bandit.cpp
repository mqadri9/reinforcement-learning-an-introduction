/*
 * Bandit.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: moham
 */

#include "Bandit.h"
#include "SoftmaxSelection.h"

Bandit::Bandit(BanditParams bandit_params): bandit_params(bandit_params){
	// TODO Auto-generated constructor stub
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	xt::random::seed(seed);
	indices = xt::arange(bandit_params.k_arm);
}

Bandit::~Bandit() {
	// TODO Auto-generated destructor stub
}


void Bandit::reset() {
	// q* values are selected from a normal distribution with mean 0 and variance 1
	q_true = xt::random::randn<double>({bandit_params.k_arm}) + bandit_params.true_rewards;
	//for (auto& el : q_true) {std::cout << el << ", " << std::endl;; }
	// current estimates for each actions
	q_estimation = xt::zeros<double>({bandit_params.k_arm}) + bandit_params.initial;

	// number of times each action has been taken
	action_count = xt::zeros<int>({bandit_params.k_arm});

	// array of best actions to take given q* values
	best_action = xt::argmax(q_true);
}

int Bandit::findMax(xt::xarray<double> xarr) {
	std::vector<int> i_vector;
	xt::xarray<int> i_xarray;
	auto xt_q_best = xt::amax(xarr);
	float q_best = (float)xt_q_best(0);
	//std::cout << "Maximums:" << q_best << std::endl;

	for (int i=0; i< bandit_params.k_arm; i++) {
		if(fabs((float)xarr(i) - q_best) < 0.00001) {
			i_vector.push_back(i);
		}
	}
	// convert i_vector to an xarray type
	i_xarray = xt::adapt(i_vector, {i_vector.size()});

	auto a = xt::random::choice(i_xarray, 1);
	return (int)a(0);
}

int Bandit::act_gradient() {
	auto exp_est = xt::exp(q_estimation);
	action_prob = xt::eval(exp_est/xt::sum(exp_est));
	SoftmaxSelection s;
	/*std::cout << "-----------------------------------" << std::endl;
	std::cout << "true rewards:" << std::endl;
	for (auto& el : q_true) {std::cout << el << ", " ; }
	std::cout << std::endl;
	std::cout << "estimations: " << std::endl;
	for (auto& el : q_estimation) {std::cout << el << ", " ; }
	std::cout << std::endl;
	std::cout << "action_prob: " << std::endl;
	for (auto& el : action_prob) {std::cout << el << ", " ; }
	std::cout << std::endl; */
	int r = s.select(action_prob);
	//std::cout << std::endl;
	//std::cout << "Action selected: " << r << std::endl;
	return r;
};

int Bandit::act_ucb() {
	auto UCB_estimation = q_estimation + bandit_params.UCB_params * xt::sqrt(std::log(xtime + 1) / (action_count + 0.00001));
	xt::xarray<double> xarr_UCB_estimation = xt::eval(UCB_estimation);
	int r = findMax(xarr_UCB_estimation);
	return r;
}

int Bandit::act_non_greedy() {
	// select a random element from action array
	xt::xarray<int> r = xt::random::choice(indices, 1);
	return (int)r(0);
}

int Bandit::act_greedy() {
	/*std::cout << "-----------------------------------" << std::endl;
	std::cout << "true rewards:" << std::endl;
	for (auto& el : q_true) {std::cout << el << ", " ; }
	std::cout << std::endl;
	std::cout << "best_action " << best_action << std::endl;
	std::cout << "estimations: " << std::endl;
	for (auto& el : q_estimation) {std::cout << el << ", " ; }
	std::cout << std::endl; */
	double m = findMax(q_estimation);
	//std::cout << std::endl;
	//std::cout << "MAX " << m << std::endl;
	return m;
}

// choose an action and return it's index
int Bandit::act() {
	auto r = xt::random::rand<double>({1});
	float e = (float)r(0);

	// choose a random action to take
	if (e < bandit_params.epsilon) {
		return act_non_greedy();
	}
	if(bandit_params.UCB_params != -1)  {
		return act_ucb();
	}
	if(bandit_params.gradient) {
		return act_gradient();
	}
	// else choose an action with the highest estimate randomly
	return act_greedy();
}

float Bandit::step(int action) {
	// reward generated from N(real_reward, 1)
	auto r = xt::random::rand<double>({1}) + q_true(action);
	float reward = (float)r(0);
	//std::cout << std::endl;
	//std::cout << "reward  " << reward << std::endl;
	xtime += 1;
	action_count(action) += 1;
	average_reward = average_reward + (reward - average_reward) / xtime;
	//std::cout << std::endl;
	//std::cout << "Average reward  " << average_reward << std::endl;
	if(bandit_params.sample_averages) {
		q_estimation(action) = q_estimation(action) + (reward - q_estimation(action)) / action_count(action);
	}
	else if(bandit_params.gradient) {
		xt::xarray<double> one_hot = xt::zeros<double>({bandit_params.k_arm});
		one_hot(action) = 1;
		//std::cout << "one_hot: " << std::endl;
		//for (auto& el : one_hot) {std::cout << el << ", " ; }
		//std::cout << std::endl;
		float baseline;
		if (bandit_params.gradient_baseline) {
			baseline = average_reward;
		}
		else {
			baseline = 0;
		}
		//std::cout << std::endl;
		//std::cout << "stepsize  " << bandit_params.step_size << std::endl;
		q_estimation = q_estimation + bandit_params.step_size*(reward - baseline) * (one_hot - action_prob);
	}
	else {
		q_estimation(action) = q_estimation(action) + bandit_params.step_size * (reward - q_estimation(action));
	}
	return reward;
}
