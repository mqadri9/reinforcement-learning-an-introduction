//============================================================================
// Name        : grid_world.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// python implementention can be found here: https://github.com/ShangtongZhang/reinforcement-learning-an-introduction/blob/master/chapter03/grid_world.py
//============================================================================

#include <iostream>
#include <array>
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
using namespace std;

const int WORLD_SIZE = 5;
const std::array<int, 2> A_POS = {0, 1};
const std::array<int, 2> A_PRIME_POS = {4, 1};
const std::array<int, 2> B_POS = {0, 3};
const std::array<int, 2> B_PRIME_POS = {2, 3};
const double DISCOUNT = 0.9;
const std::array<std::array<int, 2>, 4> ACTIONS = {{{{0, -1}}, {{-1, 0}}, {{0, 1}}, {{1, 0}}}};
const double ACTION_PROB = 0.25;

struct retStep {
	std::array<int, 2> nextState;
	double reward;
};

retStep step(std::array<int, 2> state, std::array<int, 2> action) {
	retStep retstep;
	std::array<int, 2> nextState;
	if(state==A_POS) {
		retstep.nextState = A_PRIME_POS;
		retstep.reward = 10;
		return retstep;
	}
	if(state==B_POS) {
		retstep.nextState = B_PRIME_POS;
		retstep.reward = 5;
		return retstep;
	}
	nextState[0] = state[0] + action[0];
	nextState[1] = state[1] + action[1];
	if(nextState[0] < 0 || nextState[0] >= WORLD_SIZE || nextState[1] < 0 || nextState[1] >= WORLD_SIZE) {
		retstep.reward = -1.0;
		retstep.nextState = state;
	}
	else {
		retstep.reward = 0;
		retstep.nextState = nextState;
	}
	return retstep;
}


void figure_3_2() {
	xt::xarray<double> value = xt::zeros<double>({WORLD_SIZE, WORLD_SIZE});
	while(true) {
		xt::xarray<double> new_value = xt::zeros<double>({WORLD_SIZE, WORLD_SIZE});
		for(int i=0; i<WORLD_SIZE; i++) {
			for(int j=0; j<WORLD_SIZE; j++) {
				for(int a=0; a<4;a++){
					retStep r;
					r = step({{i,j}}, ACTIONS[a]);
					new_value(i, j) = new_value(i, j) + ACTION_PROB * (r.reward + DISCOUNT * value(r.nextState[0], r.nextState[1]));
				}
			}
		}
		auto sum = xt::eval(xt::sum(xt::abs(value - new_value)));
		float p = (float)sum(0);
		if (p < 0.0001) {
			break;
		}
		value = new_value;
	}
	std::cout << value << std::endl;
}

void figure_3_5() {
	xt::xarray<double> value = xt::zeros<double>({WORLD_SIZE, WORLD_SIZE});
		while(true) {
			xt::xarray<double> new_value = xt::zeros<double>({WORLD_SIZE, WORLD_SIZE});
			for(int i=0; i<WORLD_SIZE; i++) {
				for(int j=0; j<WORLD_SIZE; j++) {
					std::array<double, 4> values;
					for(int a=0; a<4;a++){
						retStep r;
						r = step({{i,j}}, ACTIONS[a]);
						values[a] = (r.reward + DISCOUNT * value(r.nextState[0], r.nextState[1]));
					}
					new_value(i, j) = *std::max_element(values.begin(), values.end());
				}
			}
			auto sum = xt::eval(xt::sum(xt::abs(value - new_value)));
			float p = (float)sum(0);
			if (p < 0.0001) {
				break;
			}
			value = new_value;
		}
		std::cout << value << std::endl;
}

int main() {
	figure_3_2();
	// V* =
	//{{ 3.309034,  8.789329,  4.427657,  5.322405,  1.492216},
	// { 1.521625,  2.992355,  2.250177,  1.907609,  0.54744 },
	// { 0.05086 ,  0.738208,  0.673151,  0.358224, -0.403104},
	// {-0.973555, -0.435458, -0.354845, -0.585568, -1.183038},
	// {-1.857663, -1.345194, -1.22923 , -1.422881, -1.975142}}

	figure_3_5();
	// V* =
	//{{ 21.977443,  24.419382,  21.977443,  19.419382,  17.477443},
	// { 19.779699,  21.977443,  19.779699,  17.801706,  16.021535},
	// { 17.801706,  19.779699,  17.801706,  16.021535,  14.419382},
	// { 16.021535,  17.801706,  16.021535,  14.419382,  12.977443},
	// { 14.419382,  16.021535,  14.419382,  12.977443,  11.679699}}

	return 0;
}
