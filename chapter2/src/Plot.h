/*
 * Plot.h
 *
 *  Created on: Jun 23, 2019
 *      Author: moham
 */

#ifndef PLOT_H_
#define PLOT_H_

#include <python3.6m\Python.h>
#include <stdexcept>
#include <stdio.h>
#include "Plot.h"
#include "Bandit.h"
#include <vector>
#include <iostream>

class Plot {
public:
	Plot();
	virtual ~Plot();
	void figure_2_1();
	void figure_2_2(retSimulate retsimulate, float epsilons[], int rows, int time);
	void figure_2_3(retSimulate retsimulate, int rows, int time);
	void figure_2_4(retSimulate retsimulate, int rows, int time);
	void figure_2_5(retSimulate retsimulate, int rows, int time);
	void figure_2_6(xt::xarray<double> rewards, xt::xarray<double> parameters, int size);
};

#endif /* PLOT_H_ */
