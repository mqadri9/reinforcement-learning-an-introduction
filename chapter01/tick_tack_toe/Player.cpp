/*
 * Player.cpp
 *
 *  Created on: May 4, 2019
 *      Author: mqadri
 */

#include "Player.h"
#include <fstream>
#include <algorithm>    // std::random_shuffle
#include <random>
#include <cstdlib>
#include <ctime>        // std::time

Player::Player() {

}

Player::Player(double step_size, double epsilon, std::map<int, std::pair<State, int>> all_states) {
	this->epsilon = epsilon;
	this->step_size = step_size;
	this->all_states = all_states;
	this->symbol = NULL;
	this->rng = std::default_random_engine {};
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
	}
}

bool Player::save_policy() {
	std::ofstream ofile;
	std::string filename;
	if (symbol == 1) {
		filename = "policy_first.txt";
	}
	else {
		filename = "policy_second.txt";
	}
	ofile.open(filename.c_str());
	if(!ofile) {
	        return false;           //file does not exist and cannot be created.
	 }
	for(std::map <int, double>::const_iterator iter= estimations.begin(); iter!=estimations.end(); ++iter) {
	        ofile<<iter->first<<"|"<<iter->second;
	        ofile<<"\n";
	}
	return true;
}

bool Player::load_policy() {
	std::string filename;
	if (symbol == 1) {
		filename = "policy_first.txt";
	}
	else {
		filename = "policy_second.txt";
	}
	std::ifstream ifile;
	ifile.open(filename.c_str());
	if(!ifile) {
		return false;
	}
	std::string line;
	std::string key;
	estimations.clear();
	while(ifile >> line) {
		size_t index_splitter = line.find('|');
		int hash = std::stoi(line.substr(0, index_splitter));
		double estimate = std::atof(line.substr(index_splitter + 1, line.length()).c_str());
		std::pair<int, double> key_val = {hash, estimate};
		estimations.insert(key_val);
	}
	return true;
}

static bool compareActionSets(std::pair<double, std::pair<int, int>> a1, std::pair<double, std::pair<int, int>> a2) {
	double estimate1 = std::get<0>(a1);
	double estimate2 = std::get<0>(a2);
	return (estimate1 < estimate2);
}

returnAct Player::act() {
	State state = states[states.size() - 1];
	//std::cout << state.data << std::endl;
	//std::cout << "Original state" << std::endl;
	std::vector<int> next_states;
	std::vector<std::pair<int, int>> next_positions;
	returnAct action;
	for(int i=0; i<state.BOARD_ROWS; i++) {
		for(int j=0; j<state.BOARD_COLS; j++) {
			// if 0 then it is a possible move
			if(state.data(i, j) == 0) {
				std::pair<int, int> pos = {i, j};
				next_positions.push_back(pos);
				State newState;
				state.getNextState(state, newState, i, j, symbol);
				int hash = newState.hashState();
				next_states.push_back(hash);
				//std::cout << newState.data << std::endl;
				//std::cout << "i: " << i << " j: " << j << std::endl;
			}
		}
	}

	// with probability epsilon, select a random non-greedy move
	double r = ((double) rand() / (RAND_MAX));
	if(r < epsilon) {
		double indexNextMove = rand() % (next_states.size());
		int row = std::get<0>(next_positions[indexNextMove]);
		int column = std::get<1>(next_positions[indexNextMove]);
		action.row = row;
		action.column = column;
		action.symbol = symbol;
		greedy[greedy.size() - 1] = 0;
		return action;
	}
	std::vector<std::pair<double, std::pair<int, int>>> values;
	for(unsigned int i=0; i<next_states.size(); i++) {
		values.push_back({estimations.find(next_states[i]) -> second, next_positions[i]});
	}
	/*std::cout << "Before shuffle................" << std::endl;
	for(std::vector<std::pair<double, std::pair<int, int>>>::iterator it=values.begin(); it!=values.end(); it++){
		std::cout << "Estimated: " << std::get<0>(*it) << " row: " << std::get<0>(std::get<1>(*it)) << " column: " << std::get<1>(std::get<1>(*it));
		std::cout << std::endl;
	}*/
	std::random_shuffle( values.begin(), values.end());
	/*std::cout << "After shuffle................" << std::endl;
	for(std::vector<std::pair<double, std::pair<int, int>>>::iterator it=values.begin(); it!=values.end(); it++){
		std::cout << "Estimated: " << std::get<0>(*it) << " row: " << std::get<0>(std::get<1>(*it)) << " column: " << std::get<1>(std::get<1>(*it));
		std::cout << std::endl;
	}*/

	std::sort(values.begin(), values.end(), compareActionSets);
	/*std::cout << "After sort................" << std::endl;
	for(std::vector<std::pair<double, std::pair<int, int>>>::iterator it=values.begin(); it!=values.end(); it++){
		std::cout << "Estimated: " << std::get<0>(*it) << " row: " << std::get<0>(std::get<1>(*it)) << " column: " << std::get<1>(std::get<1>(*it));
		std::cout << std::endl;
	}*/

	std::reverse(values.begin(), values.end());
	/*std::cout << "After reverse................" << std::endl;
	for(std::vector<std::pair<double, std::pair<int, int>>>::iterator it=values.begin(); it!=values.end(); it++){
		std::cout << "Estimated: " << std::get<0>(*it) << " row: " << std::get<0>(std::get<1>(*it)) << " column: " << std::get<1>(std::get<1>(*it));
		std::cout << std::endl;
	}*/
	int row = std::get<0>(std::get<1>(values[0]));

	int column = std::get<1>(std::get<1>(values[0]));
	action.row = row;
	action.column = column;
	action.symbol = symbol;
	return action;
}








