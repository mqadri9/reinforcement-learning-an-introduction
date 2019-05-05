/*
 * HumanPlayer.cpp
 *
 *  Created on: May 4, 2019
 *      Author: mqadri
 */

#include "HumanPlayer.h"
#include "State.h"


HumanPlayer::HumanPlayer() {
	symbol = NULL;
}

HumanPlayer::~HumanPlayer() {
	// TODO Auto-generated destructor stub
}

void HumanPlayer::reset(){

}

void HumanPlayer::setState(State newState){
	state = newState;
}

void HumanPlayer::setSymbol(int newSymbol){
	symbol = newSymbol;
}

void HumanPlayer::backup(){

}

returnAct HumanPlayer::act(){
	char input;
	std::cout << "Enter your position" << std::endl;
	std::cin >> input;
	bool found = false;
	int index;
	int row;
	int column;
	for(int i=0; i< 9; i++) {
		if(keys[i] == input) {
			found = true;
			index = i;
			break;
		}
	}
	if (!found) {
		throw "Invalid input by user";
	}
	row = index / int(state.BOARD_COLS);
	column = index % state.BOARD_COLS;
	if(state.data(row, column) != 0) {
		std::cout << "Invalid Input" << std::endl;
		act();
	}
	returnAct result = {row, column, symbol};
	return result;
}

