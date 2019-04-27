/*
 * State.cpp
 *
 *  Created on: Apr 22, 2019
 *      Author: mqadri
 */

#include "State.h"
#include <iostream>
#include <vector>

State::State(): hashval(0), winner(0), end(0) {
	data = xt::zeros<int>({3,3});
}

State::~State() {
	// TODO Auto-generated destructor stub

}

int State::hashState(){
	int tmp;
	if(!hashval) {
		hashval = 0;
		data.reshape({BOARD_SIZE});
		for (int i = 0; i<BOARD_SIZE; i++){
			if(data(i) == -1){
				tmp = 2;
			}
			else{
				tmp = data(i);
			}
			hashval = hashval * 3 + tmp;
		}
	}
	return hashval;
}

void State::helperIsEnd(int p) {
	if( p == 3 ) {
		end = 1;
		winner = 1;
	}
	else if( p == -3 ) {
		end = 1;
		winner = -1;
	}
	else if (p==BOARD_SIZE) {
		winner = 0;
		end = 1;
	}
}

bool State::isEnd() {
	if(end != 0) {
		return end;
	}
	std::vector<xt::xarray<int>> res;
	int diag1_sum = 0;
	int diag2_sum = 0;
	for(int i=0; i< BOARD_ROWS; i++) {
		diag1_sum+=data(i,i);
		diag2_sum+=data(i, BOARD_ROWS - i - 1);
	}
	helperIsEnd(diag1_sum);
	if(end != 0 ) {
		return end;
	}
	helperIsEnd(diag2_sum);
	if(end != 0 ) {
		return end;
	}
	res.push_back(xt::sum(data, 0));
	res.push_back(xt::sum(data, 1));
	for(std::vector<xt::xarray<int>>::iterator it=res.begin(); it != res.end(); it++) {
		for(int col = 0 ; col < BOARD_ROWS; col++) {
			helperIsEnd((*it)[col]);
			if(end != 0 ) {
				return end;
			}
		}
	}
	auto sum = xt::eval(xt::sum(xt::abs(data)));
	helperIsEnd((int)sum(0));
	if(end != 0 ) {
		return end;
	}
	return end;
}
