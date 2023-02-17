#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "../headers/lib.h"

int read_sparse_from_file(const char *filename, csr_vector_t *A)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return perror("Failed: "), -1;

    int buf;
    fscanf(f, "%d %d %d\n", &(A->nb), &(A->dim), &buf);

    A->val = malloc(sizeof(double) * A->nb);
    A->rows = malloc(sizeof(int) * (A->dim+1));
    A->cols = malloc(sizeof(int) * A->nb);

    for (int i = 0; i < A->nb; i++)
        fscanf(f, "%lf ", &(A->val[i]));
    fscanf(f, "\n", &buf);

    for (int i = 0; i < A->dim+1; i++)
        fscanf(f, "%d ", &(A->rows[i]));
    fscanf(f, "\n", &buf);

    for (int i = 0; i < A->nb; i++)
        fscanf(f, "%d ", &(A->cols[i]));

    fclose(f);

    return 0;
}

void mult_mat_CSR_vect(const csr_vector_t *A, double *x)
{
	int n = A->dim;
	double *tmp = calloc(n, sizeof(double));

	for (int i = 0; i < n; ++i)
		for (int j = A->rows[i]; j < A->rows[i+1]; ++j)
			tmp[i] += A->val[j] * x[A->cols[j]];

	for(int i = 0; i < n; ++i)
		x[i] = tmp[i];

	free(tmp);
}

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

double *PageRank(csr_vector_t *A, const double epsilon, const double beta)
{
	int i = 0, n = A->dim;
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
		mult_mat_CSR_vect(A, x);
		// for (int i = 0; i < n; ++i)
		// 	printf("%lf ", x[i]);
		// printf("\n");

		// Compute teleportations
		double norm1 = 0.0;
		for (int j = 0; j < n; ++j) {
			const double val = beta * x[j] + e[j];
			x[j] = val;
			// Reduce the x vector
			norm1 += val;
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

		// loop = 0;
	}

	free(e);
	free(old_x);
	return x;
}