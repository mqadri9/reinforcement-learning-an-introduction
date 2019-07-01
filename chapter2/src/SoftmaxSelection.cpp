/*
 * SoftmaxSelection.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: moham
 */

#include "SoftmaxSelection.h"

SoftmaxSelection::SoftmaxSelection() {
	// TODO Auto-generated constructor stub
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	xt::random::seed(seed);
}

SoftmaxSelection::~SoftmaxSelection() {
	// TODO Auto-generated destructor stub
}

int SoftmaxSelection::select(xt::xarray<double> probs){
	// calculate the cdf and return an action index based on the softmax distribution
	auto r = xt::random::rand<double>({1});
	double cur_cutoff = 0;
	float f = (float)r(0);
    for(int i=0; i<probs.size()-1; ++i) {
        cur_cutoff += probs[i];
        if(f < cur_cutoff) return i;
    }
    return probs.size()-1;
}
