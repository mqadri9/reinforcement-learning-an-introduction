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
#include <iostream>

class State {
public:
	xt::xarray<double> data;
	int hashval;
	short int winner;
	short int end;
	int BOARD_ROWS = 3;
	int BOARD_COLS = 3;
	int BOARD_SIZE = BOARD_ROWS * BOARD_COLS;

private:
	void helperIsEnd(int p);

public:
	State();
	void getNextState(const State &obj, State& nextState, int i, int j, int symbol);
	virtual ~State();
	int hashState();
	short int isEnd();
	void printState();
	friend std::ostream& operator<<(std::ostream& os, const State& s);
};


#endif /* STATE_H_ */
