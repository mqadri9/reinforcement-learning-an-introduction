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
	return (int)hashval;
}

void State::helperIsEnd(int p) {
	//std::cout << p;
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

short int State::isEnd() {
	data.reshape({BOARD_ROWS, BOARD_COLS});
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
	int p = (int)sum(0);
	if (p==BOARD_SIZE) {
		winner = 0;
		end = 1;
	}
	return end;
}

void State::getNextState(const State &obj, State& nextState, int i, int symbol) {
	nextState.data = obj.data;
	nextState.data(i) = symbol;
}

void State::printState() {
	int k = 0;
	std::cout << "----------" << std::endl;
	while (k < BOARD_ROWS) {
		std::string out = "| ";
		for(int i = 3*k; i<BOARD_COLS + 3*k; i++) {
			std::string token;
			if(data(i) == 1) {
				token = "*";
			}
			else if(data(i) == -1){
				token = "0";
			}
			else {
				token = "x";
			}
			out += token + "| ";
		}
		std::cout << out << std::endl;
		std::cout << "----------" << std::endl;
		k++;
	}
}

std::ostream& operator<<(std::ostream& os, const State& s){
	    os << s.data;
	    return os;
}
