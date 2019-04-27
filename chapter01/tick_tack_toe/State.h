/*
 * State.h
 *
 *  Created on: Apr 22, 2019
 *      Author: mqadri
 */

#ifndef STATE_H_
#define STATE_H_

#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include <complex>
#include "xtensor/xcomplex.hpp"
#include "xtensor/xbuilder.hpp"
#include "xtensor/xtensor.hpp"

class State {
private:
	const int BOARD_ROWS = 3;
	const int BOARD_COLS = 3;
	const int BOARD_SIZE = BOARD_ROWS * BOARD_COLS;

public:
	xt::xarray<double> data;
	int hashval;
	short int winner;
	short int end;

private:
	void helperIsEnd(int p);

public:
	State();
	virtual ~State();
	int hashState();
	bool isEnd();
};


#endif /* STATE_H_ */
