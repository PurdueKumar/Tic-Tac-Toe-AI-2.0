#include "game.h"
#include <iostream>

game::game() {

	this->zeros = 9;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			this->grid[i][j] = 0;

		}

	}

}

game::~game() {

}

void game::newGame() {

	this->zeros = 9;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			this->grid[i][j] = 0;

		}

	}
}
int game::checkStat() {

//check columns
	for (int i = 0; i < 3; i++) {

		if (this->grid[i][0] == 0) {

			continue;

		}

		else {

			int n = grid[i][0];
			int j = 1;
			while (grid[i][j] == n) {

				j++;
				if (j == 3) {

					return n;
				}

			}
		}

	}

//check rows
	for (int j = 0; j < 3; j++) {

		if (this->grid[0][j] == 0) {

			continue;

		}

		else {

			int n = grid[0][j];
			int i = 1;
			while (grid[i][j] == n) {

				i++;
				if (i == 3) {

					return n;
				}

			}
		}

	}

//check diags

	if (this->grid[0][0] != 0) {

		int n = this->grid[0][0];
		int i = 1;
		int j = 1;

		while (this->grid[i][j] == n) {

			i++;
			j++;

			if (i == 3) {

				return n;
			}

		}

	}

	if (this->grid[0][2] != 0) {

		int n = this->grid[0][2];
		int i = 1;
		int j = 1;

		while (this->grid[i][j] == n) {

			i++;
			j--;

			if (i == 3) {

				return n;
			}

		}

	}

	//check draw
	if (this->zeros == 0) {

		return 3;

	}

	return 0;

}
void game::getMoves(int move,int marker) {

	grid[move/3][move % 3] = marker;
	this->zeros--;

}

int* game::possibleMoves() {

	int * pMoves = new int[10];

	int z = this->zeros;
	int ind = 1;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			if (this->grid[i][j] == 0) {

				pMoves[ind] = (3 * i) + j;
				ind++;

			}

		}

	}

	pMoves[0] = z;

	return pMoves;
}
void game::printGrid() {

	for (int i = 0; i < 3; i++) {

		cout << "-------" << endl;

		for (int j = 0; j < 3; j++) {

			cout << "|";
			switch (this->grid[i][j]) {

			case 0:
				cout << " ";
				break;
			case 1:
				cout << "X";
				break;
			case 2:
				cout << "O";
				break;
			}

		}

		cout << "|" << endl;
	}
	cout << "-------"<<endl;
}

int* game::copyGrid() {

	int * cpy = new int[9];

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			cpy[i * 3 + j] = grid[i][j];

		}

	}

	return cpy ;
}