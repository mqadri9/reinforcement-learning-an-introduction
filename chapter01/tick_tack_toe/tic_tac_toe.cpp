/*###############################################################################################################
This code was ported to c++ from python.																		#
Original code can be found here:																				#
https://github.com/ShangtongZhang/reinforcement-learning-an-introduction/blob/master/chapter01/tic_tac_toe.py   #
# Copyright (C)                                                                                              	#
# 2016 - 2018 Shangtong Zhang(zhangshangtong.cpp@gmail.com)             										#
# 2016 Jan Hakenberg(jan.hakenberg@gmail.com)                         											#
# 2016 Tian Jun(tianjun.cpp@gmail.com)                                											#
# 2016 Kenta Shimada(hyperkentakun@gmail.com)                         											#
# Permission given to modify the code as long as you keep this       											#
# declaration at the top                                              											#
#################################################################################################################
*/

#include <iostream>
#include "State.h"
#include "HumanPlayer.h"
#include "Player.h"
#include "Judger.cpp"

void train(int epochs, int print_every_n, std::map <int, std::pair<State, int>>& all_states) {
	Player player1(0.01 ,0.1, all_states);
	Player player2(0.01 ,0.1, all_states);
	Judger<Player, Player> judger(&player1, &player2);
	double player1_win = 0.0;
	double player2_win = 0.0;
	for(int i=1; i<=epochs; i++) {
		int winner = judger.play(false, all_states);
		if(winner == 1){
			player1_win++;
		}
		if(winner == -1){
			player2_win++;
		}
		if (i % print_every_n == 0){
			std::cout << "Epoch: " << i << " , player 1 winrate: " <<  player1_win/i << " , player 2 winrate: " <<  player2_win/i << std::endl;
		}
		player1.updateEstimates();
		player2.updateEstimates();
		judger.reset();
	}
	player1.save_policy();
	player2.save_policy();
}

void compete(int turns, std::map <int, std::pair<State, int>>& all_states) {
	Player player1(0 ,0, all_states);
	Player player2(0 ,0, all_states);
	Judger<Player, Player> judger(&player1, &player2);
	player1.load_policy();
	player2.load_policy();
	double player1_win = 0.0;
	double player2_win = 0.0;
	for(int i=0; i<turns; i++) {
		int winner = judger.play(false, all_states);
		if(winner == 1){
			player1_win++;
		}
		if(winner == -1){
			player2_win++;
		}
		judger.reset();
	}
	std::cout << "Turns: " << turns << " , player 1 winrate: " <<  player1_win/turns << " , player 2 winrate: " <<  player2_win/turns << std::endl;
}


void play(std::map <int, std::pair<State, int>>& all_states){
	while(true) {
		HumanPlayer player1;
		Player player2(0 ,0, all_states);
		Judger<HumanPlayer, Player> judger(&player1, &player2);
		player2.load_policy();
		int winner = judger.play(false, all_states);
		if(winner == player2.symbol) {
			std::cout << player1.state.data << std::endl;
			std::cout << " YOU LOSE " << std::endl;
		}
		else if(winner == player1.symbol) {
			std::cout << player1.state.data << std::endl;
			std::cout << " YOU WIN " << std::endl;
		}
		else {
			std::cout << player1.state.data << std::endl;
			std::cout << " YOU TIED " << std::endl;
		}
	}
}

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
	getAllStatesImpl(current_state, current_symbol, all_states);
	return all_states;
}

int main() {
	auto all_states = getAllStates();
	train(500000, 500, all_states);
	compete(1000, all_states);
	play(all_states);
	return 0;
}

