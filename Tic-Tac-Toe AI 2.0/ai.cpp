#include "ai.h"
#include <mkl.h>
#include <fstream>
#include <string.h>
#include <cmath>

using namespace std;

int ai::move(int * pMoves, int * grid) {

	reverseGrid(grid);

	double * input = (double *)mkl_malloc(sizeof(double) * 10 , 8);
	double * hunits = (double *)mkl_malloc(sizeof(double) * 51, 8);

	input[0] = 1;
	hunits[0] = 1;

	for (int i = 1; i < 10; i++) {

		input[i] = (double)grid[i - 1];

	}

	multiply(t1, input, &hunits[1], 50, 10, 1);	
	sigmoid(hunits, 51);

	multiply(t2, hunits, pred, 9, 51, 1);
	sigmoid(pred, 9);

	int m = cblas_idamax(9, pred, 1);

	for (int i = 1; i < (pMoves[0] + 1); i++) {

		if (m == pMoves[i]) {

			cout << "AI move is " << m << endl;
			mkl_free(input);
			mkl_free(hunits);
			return m;
		}

	}

	m = rand() % pMoves[0] + 1;
	cout << "random move is " << pMoves[m] << endl;
	rcount++;

	mkl_free(input);
	mkl_free(hunits);

	return pMoves[m];

}

ai::ai(int i, int h, int o, int pnum) {

	this->il = i;
	this->hl = h;
	this->ol = o;
	this->rcount = 0;
	this->pnum = pnum;

	ifstream theta1;
	ifstream theta2;

	theta1.open("Theta1win.csv");
	theta2.open("Theta2win.csv");

	t1 = (double *)mkl_malloc(h * (i + 1) * sizeof(double), 8);
	t2 = (double *)mkl_malloc(o * (h + 1) * sizeof(double), 8);
	pred = (double *)mkl_malloc(9 * sizeof(double), 8);

	if (theta1.fail() || theta2.fail()) {

		cout << "failed to open theta file\n";
		system("pause");
		exit(1);

	}

	int t1num = populateMatrix(theta1, t1);
	int t2num = populateMatrix(theta2, t2);

	if (t1num == 0) {

		cout << "no values added to matrix Theta 1" << endl;


	}

	if (t2num == 0) {

		cout << "no values added to matrix Theta 2" << endl;

	}

	theta1.close();
	theta2.close();

}

int ai::populateMatrix(ifstream& in, double * t) {

	char * line;
	char * tok;
	char * next_token;
	double num;
	int i = 0;

	line = (char *)malloc(sizeof(char) * 1000);

	while (1) {

		in.getline(line, 1000);
		if (in.eof()) {

			break;

		}

		tok = strtok_s(line, ",", &next_token);

		while (tok) {

			num = atof(tok);
			t[i] = num;
			i++;

			tok = strtok_s(NULL, ",", &next_token);

		}

	}
	free(line);

	return i;
}

void ai::sigmoid(double * m, int n) {

	for (int i = 0; i < n; i++) {
	
		m[i] = 1 / (1 + exp(-1 * m[i]));

	}

}


void ai::multiply(double * a, double * b, double * c , int m , int k , int n) {


	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, a, k, b, n, 1, c, n);

}

void ai::printr() {

	cout << "number of random AI moves: " << this->rcount << endl;
}

ai::~ai() {

	mkl_free(t1);
	mkl_free(t2);
	mkl_free(pred);


}

void ai::reverseGrid(int * grid){

	if (this->pnum == 1) {

		return;

	}

	for (int i = 0; i < 9; i++) {

		if (grid[i] == 1) {

			grid[i] = 2;

		}

		else if (grid[i] == 2) {

			grid[i] = 1;

		}

	}

}