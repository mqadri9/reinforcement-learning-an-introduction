/*
 * Player.cpp
 *
 *  Created on: May 4, 2019
 *      Author: mqadri
 */

#include "Player.h"

Player::Player(double step_size, double epsilon, std::map<int, std::pair<State, int>> all_states) {
	this->epsilon = epsilon;
	this->step_size = step_size;
	this->all_states = all_states;
	this->symbol = NULL;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::reset() {
	states.clear();
	greedy.clear();
}

void Player::setState(State newState) {
	states.push_back(newState);
	greedy.push_back(1);
}

void Player::initEstimations(int const symbol) {
	// Page 9
	for(std::map<int, std::pair<State, int>>::iterator it=all_states.begin(); it!=all_states.end(); it++){
		int hash = it->first;
		int isEnd = std::get<1>(it->second);
		State state = std::get<0>(it->second);
		std::pair<int, double> key_val;
		if (isEnd) {
			if(state.winner == symbol) {
				key_val = {hash, 1.0};
				estimations.insert(key_val);
			}
			else if(state.winner == 0) {
				key_val = {hash, 0.5};
				estimations.insert(key_val);
			}
			else {
				key_val = {hash, 0};
				estimations.insert(key_val);
			}
		}
		else {
			key_val = {hash, 0.5};
			estimations.insert(key_val);
		}
		/*if (hash == 10125) {
			std::cout << "+++++++++++++++++++++hash10125" << std::endl;
			std::cout << std::get<1>(key_val) << std::endl;
			std::cout << state.data << std::endl;
		}
		if (hash == 0) {
			std::cout << "+++++++++++++++++++++hash0" << std::endl;
			std::cout << std::get<1>(key_val) << std::endl;
			std::cout << state.data << std::endl;
		}*/
	}
}

void Player::setSymbol(int newSymbol) {
	symbol = newSymbol;
	initEstimations(newSymbol);
}

void Player::updateEstimates() {
	std::cout << "********************************" << std::endl;
	// For each non greedy move, update state i to be closer to the estimate of state i+1
	// That is: if the player wins the game, update all previous states (from non-greedy moves) that led to the winning state to be closer to 1
	// If the player lost the game, update all previous states (from non-greedy moves) that led to the losing state to be closer to 0
	std::vector<int> trajectoryHashes;
	for(std::vector<State>::iterator it=states.begin(); it!=states.end(); it++){
		trajectoryHashes.push_back((*it).hashState());
	}
	// start i from trajectoryHashes.size() - 2 since the first state to update is last_state - 1 (last state is constant either win, lose or draw)
	//std::cout << trajectoryHashes.size();
	for(int i = trajectoryHashes.size() - 2 ;  i>=0; i--) {
		int hash = trajectoryHashes[i];
		double last_estimate = estimations.find(trajectoryHashes[i+1]) -> second;
		std::map<int, double>::iterator current_estimate = estimations.find(hash);
		double td_error = greedy[i] * (last_estimate - current_estimate->second);
		//std::cout << last_estimate << std::endl;
		//std::cout << current_estimate->second << std::endl;
		current_estimate->second += step_size*td_error;
		std::cout << "hash: " << hash << " Estimate: " << current_estimate->second << std::endl;
	}
}

void Player::save_policy() {

}

void Player::load_policy() {

}

returnAct Player::act() {

}
