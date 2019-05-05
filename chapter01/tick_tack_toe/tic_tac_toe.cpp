//============================================================================
// Name        : tic_tac_toe.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "State.h"
#include "HumanPlayer.h"
#include "Player.h"
#include "Judger.cpp"

using namespace std;

void getAllStatesImpl(State current_state, int current_symbol, std::map <int, std::pair<State, int>>& all_states) {
	int k = 0;
	while (k < current_state.BOARD_ROWS) {
		for(int i = 3*k; i<current_state.BOARD_COLS + 3*k; i++) {
			if(current_state.data(i) == 0){
				State newState;
				current_state.getNextState(current_state, newState, i, -1, current_symbol);
				int newHash = newState.hashState();
				if(all_states.find(newHash) == all_states.end()){
					short int isEnd = newState.isEnd();
					std::pair<State, int> val = {newState, isEnd};
					std::pair<int, std::pair<State, int>> key_val = {newHash, val};
					all_states.insert(key_val);
					if(!isEnd){
						getAllStatesImpl(newState, -current_symbol, all_states);
					}
				}
 			}
		}
		k++;
	}
}

std::map <int, std::pair<State, int>> getAllStates() {
	std::map <int, std::pair<State, int>> all_states;
	int current_symbol = 1;
	State current_state;
	std::pair<State, int> val = {current_state, current_state.isEnd()};
	std::pair<int, std::pair<State, int>> key_val = {current_state.hashState(), val};
	all_states.insert(key_val);
	/*for(std::map<int, std::pair<State, int>>::iterator it=all_states.begin(); it!=all_states.end(); it++){
		std::cout << it->first;
		std::cout << std::get<1>(it->second);
		std::cout << std::get<0>(it->second);
	}*/
	getAllStatesImpl(current_state, current_symbol, all_states);

	return all_states;
}

int main() {
	auto all_states = getAllStates();
	HumanPlayer player1;
	HumanPlayer player2;
	Judger<HumanPlayer, HumanPlayer> j(player1, player2);
	int winner = j.play(true, all_states);
	std::cout << "The winner is" << winner << std::endl;
	return 0;
}
