#pragma once

#include "player.h"

class ai : public player {

private:

	double * t1;
	double * t2;
	double * pred;
	int il, hl, ol;
	int rcount;
	int pnum;

	int populateMatrix(ifstream&, double *);
	void sigmoid(double *, int);
	void multiply(double *, double *, double * , int , int , int);
	void reverseGrid(int *);

public:
	ai(int i, int h, int o , int pnum);
	int move(int * , int *);
	void printr();
	~ai();
};