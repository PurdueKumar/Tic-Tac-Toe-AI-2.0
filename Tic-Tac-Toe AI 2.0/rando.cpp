#include "rando.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int rando::move(int * pMoves , int * grid) {

	int r = rand() % pMoves[0] + 1;
	cout << "move is " << pMoves[r] << endl;

	return pMoves[r];
}