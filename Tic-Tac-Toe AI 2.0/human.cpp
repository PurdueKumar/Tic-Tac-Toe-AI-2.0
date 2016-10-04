#include "player.h"
#include "human.h"

using namespace std;

void printOptions(int *);

int human::move(int * pMoves, int * grid) {

	int in;

	while (1) {
		cout << "your move options: " << endl;
		printOptions(grid);

		cout << "Enter your move: ";

		cin >> in;

		for (int i = 1; i < (pMoves[0] + 1); i++) {

			if (in == pMoves[i]) {

				cout << "move made " << in;
				return in;
			}

		}

		cout << "Invalid move" << endl;
	}

}

void printOptions(int * grid) {

	for (int i = 0; i < 9; i++) {

		if (i % 3 == 0) {
			cout << "-------" << endl;
		}

			cout << "|";
			switch (grid[i]) {

			case 0:
				cout << i;
				break;
			case 1:
				cout << "X";
				break;
			case 2:
				cout << "O";
				break;
			}

		
			if (i % 3 == 2) {
				cout << "|" << endl;
			}
	}
	cout << "-------" << endl;



}