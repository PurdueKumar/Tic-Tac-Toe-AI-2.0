#pragma once

#include <iostream>
#include <string>

using namespace std;

class game {

private:

	int zeros;
	int grid[3][3];

public:
	game();
	~game();

	void newGame();
	int* copyGrid();
	int checkStat();
	void getMoves(int,int);
	int* possibleMoves();
	void printGrid();

};