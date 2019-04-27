//============================================================================
// Name        : tic_tac_toe.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "State.h"
using namespace std;

int main() {
	State state;
	int end = state.isEnd();
	std::cout << end;
	return 0;
}
