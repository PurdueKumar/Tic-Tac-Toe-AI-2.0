
#include "ai.cuh"
#include <fstream>
#include <string.h>
#include <cmath>

using namespace std;

ai::ai(int i, int h, int o, int pnum) {

	cudaError_t cudaStatus;
	cublasStatus_t cublasStatus;
	cublasStatus = cublasCreate(&handle);

	this->il = i;
	this->hl = h;
	this->ol = o;
	this->rcount = 0;
	this->pnum = pnum;

	ifstream theta1;
	ifstream theta2;

	theta1.open("Theta1win.csv");
	theta2.open("Theta2win.csv");

	cudaStatus = cudaSetDevice(0);

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto cleanup;
	}

	cudaStatus = cudaMalloc((void**)&t1, (sizeof(double) * h * (i + 1)));

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc for t1 failed!");
		goto cleanup;
	}

	cudaStatus = cudaMalloc((void**)&t2, (sizeof(double) * (h+i) * o));

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc for t2 failed!");
		goto cleanup;
	}

	cudaStatus = cudaMalloc((void**)&pred, (sizeof(double) * 9));

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc for pred failed!");
		goto cleanup;
	}

	if (theta1.fail() || theta2.fail()) {

		cout << "failed to open theta file\n";
		system("pause");
		exit(1);

	}

	int t1num = populateMatrix(theta1, t1, h * (i + 1));
	int t2num = populateMatrix(theta2, t2, (h + i) * o);

	theta1.close();
	theta2.close();

cleanup:

	cudaFree(t1);
	cudaFree(t2);
	theta1.close();
	theta2.close();
}

__global__ void sigKernal(double * t) {

	int i = threadIdx.x;
	t[i] = 1 / (1 + exp(-1 * t[i]));

}

void ai::sigmoid(double * t, int n) {

	cudaError_t cudaStatus;

	sigKernal <<<1, n >>> (t);

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
			return;
	}

	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		return;
	}

	return;

}

int ai::populateMatrix(ifstream& in, double * t, int n) {

	char * line;
	char * tok;
	char * next_token;
	double num;
	int i = 0;
	double * temp = (double *)malloc(sizeof(double) * n);
	cudaError_t cudaStatus;

	line = (char *)malloc(sizeof(char) * 1000);

	while (1) {

		in.getline(line, 1000);
		if (in.eof()) {

			break;

		}

		tok = strtok_s(line, ",", &next_token);

		while (tok) {

			num = atof(tok);
			temp[i] = num;
			i++;

			tok = strtok_s(NULL, ",", &next_token);

		}

	}
	free(line);

	cudaStatus = cudaMemcpy(t, temp, n, cudaMemcpyHostToDevice);

	free(temp);

	return i;
}

void ai::reverseGrid(int * grid) {

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

void ai::multiply(double * a, double * b, double * c, int m, int k, int n) {

	cublasStatus_t cublasStatus;
	const double coeff = 1;

	cublasStatus = cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, &coeff, a, k, b, n, &coeff, c, n);

}

int ai::move(int * pMoves, int * grid) {

	cudaError_t cudaStatus;
	cublasStatus_t cublasStatus;
	double * input;
	double * hunits;

	int bias = 1;

	reverseGrid(grid);

	cudaStatus = cudaMalloc((void**)&input, (sizeof(double) * 10));
	cudaStatus = cudaMalloc((void**)&hunits, (sizeof(double) * 51));

	cudaMemcpy(&input[0], &bias, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(&hunits[0], &bias, sizeof(int), cudaMemcpyHostToDevice);

	cudaMemcpy(&input[1], &grid, sizeof(int) * 9, cudaMemcpyHostToDevice);

	multiply(t1, input, &hunits[1], 50, 10, 1);
	sigmoid(hunits, 51);

	multiply(t2, hunits, pred, 9, 51, 1);
	sigmoid(pred, 9);

	int m;
	cublasStatus = cublasIdamax(handle, 9, pred, 1 , &m);

	for (int i = 1; i < (pMoves[0] + 1); i++) {

		if (m == pMoves[i]) {

			cout << "AI move is " << m << endl;
			cudaFree(input);
			cudaFree(hunits);

			return m;
		}

	}

	m = rand() % pMoves[0] + 1;
	cout << "random move is " << pMoves[m] << endl;
	rcount++;

	cudaFree(input);
	cudaFree(hunits);

	return pMoves[m];

}

void ai::printr() {

	cout << "number of random AI moves: " << this->rcount << endl;
}


ai::~ai() {

	cudaFree(t1);
	cudaFree(t2);
	cudaFree(pred);

	cublasDestroy(handle);

	
}