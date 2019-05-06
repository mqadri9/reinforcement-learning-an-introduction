/*
 * Player.h
 *
 *  Created on: May 4, 2019
 *      Author: mqadri
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include "State.h"
#include <vector>

class Player {
public:
	double step_size;
	double epsilon;
	int symbol;
	std::map <int, double> estimations;
	std::vector<State> states;
	std::vector<int> greedy;
	std::map<int, std::pair<State, int>> all_states;

public:
	Player(double step_size, double epsilon, std::map<int, std::pair<State, int>> all_states);
	virtual ~Player();
	void reset();
	void setState(State state);
	void setSymbol(int symbol);
	void updateEstimates();
	void save_policy();
	void load_policy();
	void initEstimations(int const newSymbol);
	returnAct act();
};

#endif /* PLAYER_H_ */
