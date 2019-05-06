/*
 * HumanPlayer.h
 *
 *  Created on: May 4, 2019
 *      Author: mqadri
 */

#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_
#include "State.h"

class HumanPlayer {
public:
	int symbol;
	char keys[10] = {'q', 'w', 'e', 'a', 's', 'd', 'z', 'x', 'c', '\0'};
	State state;

public:
	HumanPlayer();
	virtual ~HumanPlayer();
	void reset();
	void setState(State state);
	void setSymbol(int symbol);
	void updateEstimates();
	returnAct act();
};

#endif /* HUMANPLAYER_H_ */
