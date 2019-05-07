/*
 * Judger.cpp
 *
 *  Created on: May 5, 2019
 *      Author: mqadri
 */

#include "Player.h"
#include "HumanPlayer.h"
#include "State.h"
#include <chrono>
#include <thread>

template<class T, class K> class Judger {
public:
	T* player1;
	K* player2;
	int current_player;
	int p1_symbol;
	int p2_symbol;
	State current_state;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

public:
	Judger(T* p1, K* p2) {
		player1 = p1;
		player2 = p2;
		current_player = -1;
		p1_symbol = 1;
		p2_symbol = -1;
		player1->setSymbol(p1_symbol);
		player2->setSymbol(p2_symbol);

		current_state = State();
	};
	void reset() {
		player1->reset();
		player2->reset();
		current_player = -1;
	}
	int play(bool print_state, const std::map<int, std::pair<State, int>>&  all_states) {
		reset();
		current_state = State();
		player1->setState(current_state);
		player2->setState(current_state);
		while(true) {
			returnAct res;
			State nextState;
			int hash;
			std::pair<State, int> val;
			int isEnd;
			if(print_state) {
				current_state.printState();
			}
			if(current_player == 1) {
				K* player = player2;
				current_player = -1;
				res = player->act(seed);
			}
			else {
				T* player = player1;
				current_player = 1;
				res = player->act(seed);
			}
			int row = res.row;
			int column = res.column;
			int symbol = res.symbol;
			current_state.getNextState(current_state, nextState, row, column, symbol);
			hash = nextState.hashState();
			val = all_states.find(hash)->second;
			isEnd = std::get<1>(val);
			current_state = std::get<0>(val);
			player1->setState(current_state);
			player2->setState(current_state);
			if(isEnd) {
				if(print_state) {
					current_state.printState();
				}
				return current_state.winner;
			}
		}

	}

};
