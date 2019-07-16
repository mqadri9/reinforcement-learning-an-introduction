//============================================================================
// Name        : gamblers_problem.cpp
// Author      : 
// Version     :
//============================================================================

#include <iostream>
#include <complex>
#include <cmath>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xcomplex.hpp"
#include "xtensor/xbuilder.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xsort.hpp"
#include "xtensor/xfixed.hpp"
#include <python3.6m\Python.h>
using namespace std;

int GOAL = 100;

// all states including state 0 and 100
xt::xarray<int> STATES = xt::arange(GOAL+1);

// probability of head
float HEAD_PROB = 0.4;

void deep_copy_xarray(xt::xarray<float> * old_value, xt::xarray<float>&value, int n1)
{
	for(int i=0; i<n1; i++) {
		(*old_value)(i) = value(i);
	}

}

// Convert a 1D xarray to a 1D python list
PyObject* xtensorToList(const xt::xarray<float> &data, int size) {
	PyObject* listObj = PyList_New(size);
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < size; i++) {
		PyObject *num = PyFloat_FromDouble( (double) data(i));
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

// Convert a 1D xarray to a 1D python list
PyObject* xtensorToList(const xt::xarray<int> &data, int size) {
	PyObject* listObj = PyList_New(size);
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < size; i++) {
		PyObject *num = PyFloat_FromDouble( (double) data(i));
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

void figure_4_3(xt::xarray<int> policy, std::vector<xt::xarray<float>> sweeps_history) {
	Py_Initialize();
 	const char *text =
						"import sys, os\n"
						"sys.path.insert(0, os.environ['WORKING_DIR'] + '\\src')\n";
	PyRun_SimpleString(text);

	PyObject *pythonSweepList = PyList_New(sweeps_history.size());

	for(int i=0 ; i< sweeps_history.size(); i++) {
		PyObject *sweep = xtensorToList(sweeps_history[i], GOAL+1);
		PyList_SET_ITEM(pythonSweepList, i, sweep);
	}

	PyObject *pythonPolicy = xtensorToList(policy, GOAL+1);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotFigure");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, pythonSweepList);
			PyTuple_SetItem(pArgs, 1, pythonPolicy);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
	Py_Finalize();
}

int main()
{
	xt::xarray<float> state_value = xt::zeros<float>({GOAL+1});
	state_value(GOAL) = 1.0;
	std::vector<xt::xarray<float>> sweeps_history;
	// value iteration
	while(true) {
		xt::xarray<float> old_state_value = xt::zeros<float>({GOAL+1});
		deep_copy_xarray(&old_state_value, state_value, GOAL+1);
		sweeps_history.push_back(old_state_value);
		for(int i=1; i<GOAL ; i++) {

			// get all possible action from current state
			// if state <=50 can bet up to state
			// if state > 50 , only need to bet up to 100-s (to reach the required the GOAL)
			int state = STATES(i);
			xt::xarray<int> actions = xt::arange(min(state, GOAL - state) + 1);
			std::vector<float> action_returns;

			// Apply The value iteration function.
			// Here we have two possible s'. Lose (with p=1-ph) and win with p=ph
			for(auto&& action: actions){
				float v = HEAD_PROB*state_value(state+action) + (1-HEAD_PROB)*state_value(state-action);
				action_returns.push_back(v);
			}
			float new_value = *std::max_element(action_returns.begin(), action_returns.end());
			state_value(state) = new_value;
		}
		auto max_value_change = xt::amax(xt::abs(state_value-old_state_value));
		float max_change = max_value_change(0);
		std::cout << "max change: " << max_change << std::endl;
		if(max_change<1e-8) {
			sweeps_history.push_back(max_change);
			break;
		}
	}
	std::cout << state_value << std::endl;
	// Compute optimal policy
	xt::xarray<int> policy = xt::zeros<int>({GOAL+1});
	for(int i=1; i<GOAL ; i++) {
		int state = STATES(i);
		xt::xarray<int> actions = xt::arange(1, min(state, GOAL - state) + 1);
		std::vector<float> action_returns;
		for(auto&& action: actions){
			float v = HEAD_PROB*state_value(state+action) + (1-HEAD_PROB)*state_value(state-action);
			action_returns.push_back(v);
		}
		// skip action=0. Add 1 to arg_max_returns since the distance function starts from index 1 (std::next)
		int arg_max_returns = std::distance(std::next(action_returns.begin()), std::max_element(action_returns.begin(), action_returns.end())) + 1;
		int new_action = actions[arg_max_returns];
		policy(state) = new_action;
	}
	figure_4_3(policy, sweeps_history);
	return 0;
}
