//============================================================================
// Name        : infinite_variance.cpp
// Author      : Mohamad Qadri
// Version     :
// Copyright (C)
// 2016-2018 Shangtong Zhang(zhangshangtong.cpp@gmail.com)
// 2016 Kenta Shimada(hyperkentakun@gmail.com)
// Permission given to modify the code as long as you keep this declaration at the top
// Description : Infinite_variance.py based on the python implementation found here:
// https://github.com/ShangtongZhang/reinforcement-learning-an-introduction/blob/master/chapter05/infinite_variance.py
//============================================================================

#include <iostream>
#include <vector>
#include <random>
#include <python3.6m\Python.h>
#include <math.h>

std::random_device rd;
std::default_random_engine generator(rd());
std::binomial_distribution<int> distribution(1,0.5);
std::binomial_distribution<int> distribution2(1,0.9);


PyObject* vectorToList_Double(const std::vector<std::vector<float>> &data, int row, int column) {
	PyObject* listRow = PyList_New(row);
	for (int i=0; i<row;i++) {
		PyObject* listCol = PyList_New(column);
		for (int j = 0; j < column; j++) {
			PyObject *num = PyFloat_FromDouble( (double) data[i][j]);
			if (!num) {
				Py_DECREF(listCol);
				throw std::logic_error("Unable to allocate memory for Python list");
			}
			PyList_SET_ITEM(listCol, j, num);
		}
		PyList_SET_ITEM(listRow, i, listCol);
	}
	return listRow;
}

struct play_return
{
	std::vector<int> trajectory;
	int reward;
};

// ACTION_END is the action of going right (to terminal state)
// ACTION_LOOP is the action of going left
int ACTION_END = 1;
int ACTION_LOOP = 0;

// The behavior policy has equal probability of selecting the left or right action.
// This behavior can be modeled using a binomial distribution with parameters t=1 and p=0.5
float behavior_policy()
{
	return distribution(generator);
}

std::vector<float> calculate_estimate(std::vector<float> &accumulated_rewards, int episodes)
{
	std::vector<float> estimates;
	for(int i = 1; i <= episodes; i++) {
		estimates.push_back(accumulated_rewards[i]/i);
	}
	return estimates;
}

std::vector<float> accumulate(std::vector<float> &rewards)
{
	std::vector<float> accumulated_rewards;
	accumulated_rewards.push_back(rewards[0]);
	for(int i=1; i <= rewards.size(); i++) {
		accumulated_rewards.push_back(rewards[i] + accumulated_rewards[i-1]);
	}
	return accumulated_rewards;
}

// Target policy has 0 probability of selecting ACTION_END
//int target_policy()
//{
//	return ACTION_LOOP;
//}

// play a game following the behavior policy
play_return play()
{
	play_return ret;
	std::vector<int> trajectory;
	while(true) {
		int action = behavior_policy();
		trajectory.push_back(action);
		if(action == ACTION_END) {
			// if the behavior policy selects the action (RIGHT), then the episode ends with 0 rewards
			ret.trajectory = trajectory;
			ret.reward = 0;
			return ret;
		}
		// at this point the agent has selected the left action, get state to go to next
		int next_state = distribution2(generator);
		if (next_state == 0) {
			ret.trajectory = trajectory;
			ret.reward = 1;
			return ret;
		}
	}
}

int main()
{
	int runs = 10;
	int episodes = 100000;
	std::vector<std::vector<float>> estimations_vector;
	for(int run=0; run< runs; run++) {
		std::vector<float>rewards;
		for(int episode=0; episode < episodes; episode++) {
			play_return ret = play();
			float rho;
			// find the importance sampling ratio
			if(ret.trajectory[ret.trajectory.size()-1] == ACTION_END) {
				// if the last action by the behavior policy agent is right, the rho = 0 since PI(right|s) = 0
				rho = 0.0;
			}
			else {
				// If the action is left, we know that PI(left|s) = 1, and B(left|s) = 0.5
				// so rho = 1/(0.5)^(size of trajectory)
				rho = 1.0/pow(0.5, ret.trajectory.size());
			}
			rewards.push_back(rho*ret.reward);
		}


		std::vector<float> accumulated_rewards = accumulate(rewards);
		std::vector<float> estimations = calculate_estimate(accumulated_rewards, episodes);
		estimations_vector.push_back(estimations);
	}
	Py_Initialize();
	const char *text =
						"import sys, os\n"
						"sys.path.insert(0, os.environ['WORKING_DIR'])\n";
	PyRun_SimpleString(text);

	PyObject* pythonEstimateValues = vectorToList_Double(estimations_vector, runs, episodes);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotFigure");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, pythonEstimateValues);
			pValue = PyObject_CallObject(pFunc, pArgs);
		}
	}
	Py_Finalize();


	return 0;
}
