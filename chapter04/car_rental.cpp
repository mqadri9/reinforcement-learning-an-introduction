//============================================================================
// Name        : car_rental.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
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

// maximum number of cars at each location
int MAX_CARS = 20;

// maximum number of cars to move during night
int MAX_MOVE_OF_CARS = 5;

// expectation for rental requests in first location
int RENTAL_REQUEST_FIRST_LOC = 3;

// expectation for rental requests in second location
int RENTAL_REQUEST_SECOND_LOC = 4;

// expectation for number of cars returned in first location
int RETURNS_FIRST_LOCATION = 3;

// expectation for numbers of cars returned in second location
int RETURNS_SECOND_LOCATION = 2;

float DISCOUNT = 0.9;

// credit earned by a car
int RENTAL_CREDIT = 10;

// credit earned for each car rental
int MOVE_CAR_COST = 2;

float INF = std::numeric_limits<float>::max();

// The list of possible actions. a positive sign indicates moving
// cars from location 1 to location 2 and vis versa for a negative sign.
xt::xarray<int> actions = xt::arange(-MAX_MOVE_OF_CARS, MAX_MOVE_OF_CARS + 1);

// a cache which holds the probability of the poisson distribution
std::map<int, float> poisson_cache;

unsigned int iter_factorial(unsigned int n)
{
    unsigned int ret = 1;
    for(unsigned int i = 1; i <= n; ++i)
        ret *= i;
    return ret;
}

// Find probability that the number of cars is n given lambda
float poisson_probability(int n, int lambda)
{
	int key = n*10 + lambda;
	if (poisson_cache.find(key) == poisson_cache.end()) {
		poisson_cache[key] = exp(n * log(lambda) - lgamma(n + 1.0) - lambda);
	}
	return poisson_cache[key];
}

void deep_copy_xarray(xt::xarray<double> * old_value, xt::xarray<double>&value, int n1, int n2)
{
	for(int i=0; i<n1; i++) {
		for (int j=0; j<n2; j++) {
			(*old_value)(i,j) = value(i,j);
		}
	}

}

float expected_return(std::array<int, 2> state, int action, xt::xarray<double> value, bool constant_returned_cars = true, bool ex = true)
{
	// state: [# of cars in first location, # of cars in second location]
	// action: positive if moving cars from first location to second location,
	//		negative if moving cars from second location to first location
	// value: state_value matrix
	// constant_returned_cars: if set True, model is simplified such that
    // the # of cars returned in daytime becomes constant
    // rather than a random value from poisson distribution, which will reduce calculation time
    // and leave the optimal policy/value state matrix almost the same

	float returns = 0.0;

	// cost for moving cars
	if(ex && action > 0){
		// 1 car moved for free from location 1 to 2
		action-=1;
		action = std::max(action, 0);
	}
	//returns -= MOVE_CAR_COST * abs(action);

	// Find # cars at each location after applying the action. (this cannot exceed MAX_CARS)
	int NUM_OF_CARS_FIRST_LOC = min(state[0] - action, MAX_CARS);
	int NUM_OF_CARS_SECOND_LOC  = min(state[1] + action, MAX_CARS);

	// Need to find V(s) = Sum over s',r { p(s', r| s, a)*[r + gamma*V(s')] }
	// Next possible states s' after applying the action and their probability can be found by applying the poisson probability for both
	// rentals P(R) and returns P(r). Then p(s', r| s, a) = P(R)*P(r)
	// P(R) = P(rental from loc 1) * P(rental from loc 2) and same for returns
	for (int rental_request_first_loc = 0; rental_request_first_loc <= 10; rental_request_first_loc++) {
		for (int rental_request_second_loc  = 0; rental_request_second_loc  <= 10; rental_request_second_loc++) {
			// Go through all possible rental requests and find the probability of this particular combination
			float prob = poisson_probability(rental_request_first_loc, RENTAL_REQUEST_FIRST_LOC) * poisson_probability(rental_request_second_loc, RENTAL_REQUEST_SECOND_LOC);
			// Truncate the probability to 0 if too small
            int num_of_cars_first_loc = NUM_OF_CARS_FIRST_LOC;
            int num_of_cars_second_loc = NUM_OF_CARS_SECOND_LOC;

            // rentals than can processed are <= num_of_cars_x_loc
            int valid_rental_first_loc = min(num_of_cars_first_loc, rental_request_first_loc);
            int valid_rental_second_loc = min(num_of_cars_second_loc, rental_request_second_loc);
			// get credits for renting
			float reward = (valid_rental_first_loc + valid_rental_second_loc) * RENTAL_CREDIT - MOVE_CAR_COST * abs(action);
			num_of_cars_first_loc -= valid_rental_first_loc;
			num_of_cars_second_loc -= valid_rental_second_loc;

			if(constant_returned_cars) {
				// get returned cars, those cars can be used for renting tomorrow
                int returned_cars_first_loc = RETURNS_FIRST_LOCATION;
                int returned_cars_second_loc = RETURNS_SECOND_LOCATION;
                num_of_cars_first_loc = min(num_of_cars_first_loc + returned_cars_first_loc, MAX_CARS);
                if(num_of_cars_first_loc > 10 && ex) {
                	reward -= 4;
                }
                num_of_cars_second_loc = min(num_of_cars_second_loc + returned_cars_second_loc, MAX_CARS);
                if(num_of_cars_second_loc > 10 && ex) {
                	reward -= 4;
                }
                returns += prob * (reward + DISCOUNT * value(num_of_cars_first_loc, num_of_cars_second_loc));
			}
		}
	}
    return returns;

}

PyObject* xtensorToList_Double(const xt::xarray<int> &data, int row, int column) {
	PyObject* listRow = PyList_New(row);
	for (int i=0; i<row;i++) {
		PyObject* listCol = PyList_New(column);
		for (int j = 0; j < column; j++) {
			PyObject *num = PyFloat_FromDouble( (double) data(i, j));
			if (!num) {
				Py_DECREF(listCol);
				throw logic_error("Unable to allocate memory for Python list");
			}
			PyList_SET_ITEM(listCol, j, num);
		}
		PyList_SET_ITEM(listRow, i, listCol);
	}
	return listRow;
}

PyObject* vectorToList_Float(const vector<float> &data) {
  PyObject* listObj = PyList_New( data.size() );
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject *num = PyFloat_FromDouble( (double) data[i]);
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

void figure_4_2(std::vector<float> iter_array, std::vector<xt::xarray<int>> policies, xt::xarray<double> value) {
	Py_Initialize();
	const char *text =
						"import sys, os\n"
						"sys.path.insert(0, os.environ['WORKING_DIR'])\n";
	PyRun_SimpleString(text);
	PyObject *pythonPolicyList = PyList_New(iter_array.size());
	for(int i=0 ; i< iter_array.size(); i++) {
		PyObject *pythonPolicy = xtensorToList_Double(policies[i], MAX_CARS, MAX_CARS);
		PyList_SET_ITEM(pythonPolicyList, i, pythonPolicy);
	}
	PyObject *pythonListIterations = vectorToList_Float(iter_array);
	PyObject *pythonValue = xtensorToList_Double(value, MAX_CARS, MAX_CARS);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotFigure");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(3);
			PyTuple_SetItem(pArgs, 0, pythonPolicyList);
			PyTuple_SetItem(pArgs, 1, pythonListIterations);
			PyTuple_SetItem(pArgs, 2, pythonValue);
			pValue = PyObject_CallObject(pFunc, pArgs);
		}
	}
	Py_Finalize();
}

int main()
{
	bool constant_returned_cars = true;
	// consider the a 2D policy matrix where:
	// row x indicates x cars available for rent in location 1
	// row y indicates y cars available for rent in location 2
	xt::xarray<double> value = xt::zeros<double>({MAX_CARS, MAX_CARS});
	xt::xarray<int> policy = xt::zeros<int>({MAX_CARS, MAX_CARS});
	float iterations = 0;
	std::vector<float> iter_array;
	std::vector<xt::xarray<int>> policies;
	// Loop until optimal policy is found
	while(true) {
		policies.push_back(policy);
		iter_array.push_back(iterations);
		// Policy evaluation
		while(true) {
			xt::xarray<double> old_value = xt::zeros<double>({MAX_CARS, MAX_CARS});
			deep_copy_xarray(&old_value, value, MAX_CARS, MAX_CARS);
			// Loop for each state
			for(int i=0; i< MAX_CARS; i++) {
				for(int j=0; j< MAX_CARS; j++) {
					// for each state, calculate the expected return given current policy and old value function
					std::array<int, 2> current_state = {i,j};
					int action = policy(i, j);
					float new_state_value =  expected_return(current_state, action, value, constant_returned_cars);
					value(i, j) = new_state_value;
				}
			}
			auto max_value_change = xt::amax(xt::abs(old_value-value));
			float max_change = max_value_change(0);
			std::cout << "max value change: " << max_change << std::endl;
			if(max_change < 0.0001) {
				break;
			}
		}
		//break;
		// Policy improvement
		bool policy_stable = true;
		for(int i=0; i< MAX_CARS; i++) {
			for(int j=0; j< MAX_CARS; j++) {
				int old_action = policy(i, j);
				std::vector<float> action_returns;
				// For each reachable action from state {i,j}, find the expected return.
				for(const int &action : actions){
					// Need number of cars available at location x > action: number of cars to be moved from x
					if((action >= 0 && action <= i) || (action >= -j and action >=0)) {
						std::array<int, 2> current_state = {i,j};
						action_returns.push_back(expected_return(current_state, action, value, constant_returned_cars));
					}
					else {
						// if not a possible action then set its return to negative infinity
						action_returns.push_back(-INF);
					}
				}
				// Improve the policy by setting policy[i,j] to the action that leads the maximum return accoding to the current value function.
				int arg_max_returns = std::distance(action_returns.begin(), std::max_element(action_returns.begin(), action_returns.end()));;
				int new_action = actions[arg_max_returns];
				policy(i, j) = new_action;
				if(old_action != new_action){
					policy_stable = false;
				}
			}
		}
		std::cout << "Policy stable: " << policy_stable << std::endl;
		if(policy_stable) {
			break;
		}
		iterations+=1;
	}
	figure_4_2(iter_array, policies, value);


	return 0;
}
