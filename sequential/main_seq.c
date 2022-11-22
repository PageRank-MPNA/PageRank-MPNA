#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void mult_mat_1D_vect(const double *A, double *x, const int n)
{
	double *tmp = calloc(n, sizeof(double));

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
			tmp[i] += A[i * n + j] * x[j];
	}

	for (int i = 0; i < n; ++i)
		x[i] = tmp[i];

	free(tmp);
}

const double norm2(const double *x, const int n)
{
	double res = 0.0;

	for (int i = 0; i < n; ++i)
		res += x[i] * x[i];


	return sqrt(res);
}

double *PageRank(double *A, const double epsilon, const double beta, const int n)
{
	int i = 0;
	double *x     = calloc(n, sizeof(double));
	double *old_x = calloc(n, sizeof(double));
	double *e     = calloc(n, sizeof(double));

	const double frac = 1.0 / (double)n;
	const double cst  = (1 - beta) * frac;

	// Init
	for (int j = 0; j < n; ++j)
	{
		x[j] = frac;
		old_x[j] = frac;
		e[j] = cst;
	}

	int loop = 1;
	while(loop)
	{
    	i++;

		// Compute multiplication matrix-vector
		mult_mat_1D_vect(A, x, n);

		// Compute teleportations
		double norm1 = 0.0;
		for (int j = 0; j < n; ++j) {
			x[j] = beta * x[j] + e[j];
			// Reduce the x vector
			norm1 += x[j];
		}

		for (int j = 0; j < n; ++j) {
			// Normalize x
			x[j] = x[j] / norm1;
			// Test break value
			old_x[j] = x[j] - old_x[j];
		}

		if (norm2(old_x, n) < epsilon)
			break;

		// Keep x vector
    	for (int j = 0; j < n; ++j)
			old_x[j] = x[j];

	}

	free(e);
	free(old_x);
	return x;
}

int main(int argc, char const *argv[])
{
	int n = 4;
	//double *matrice = calloc(n * n, sizeof(double));
	const double frac = 1.0 / 3.0;
	double matrice[16] = {0, 0, 0, 1, frac, 0, 1, 0, frac, 1, 0, 0, frac, 0, 0, 0};
	
	double beta = 0.85;
	double epsilon = 1e-4;

	double *res = PageRank(matrice, epsilon, beta, n);

	printf("\n");
	for (int i = 0; i < n; ++i)
		printf("%e ", res[i]);
	printf("\n");

	return 0;
}