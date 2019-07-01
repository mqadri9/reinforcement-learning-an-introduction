/*
 * SoftmaxSelection.h
 *
 *  Created on: Jun 23, 2019
 *      Author: moham
 */

#ifndef SOFTMAXSELECTION_H_
#define SOFTMAXSELECTION_H_
#include "xtensor/xrandom.hpp"
#include "xtensor/xarray.hpp"

class SoftmaxSelection {
public:
	SoftmaxSelection();
	virtual ~SoftmaxSelection();
	int select(xt::xarray<double> probs);
};

#endif /* SOFTMAXSELECTION_H_ */
