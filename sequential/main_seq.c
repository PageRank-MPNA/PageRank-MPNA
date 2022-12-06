#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct csr_vector_s
{
	double *val;
	int *rows;
	int *cols;
	int nb;
} csr_vector_t;


void read_sparse_from_file(const char *filename, csr_vector_t *A, const int n)
{
	FILE *f = fopen(filename, "r");

	int len_val, len_rows, len_cols;

	fscanf(f, "%d %d %d\n", &len_val, &len_rows, &len_cols);

	A->nb = len_val;
	A->val = malloc(sizeof(double) * len_val);
	A->rows = malloc(sizeof(double) * len_rows);
	A->cols = malloc(sizeof(double) * len_cols);

	fseek(f, sizeof(int) + 1, SEEK_SET);

	int i = 0, j = 0;
	double value;
	while(fscanf(f, "%lf ", &value) != EOF)
	{
		if(i < len_val)
		{
			A->val[j] = value;
			if(j == len_val - 1){
				fscanf(f, "\n");
				j = 0;
			}
			else
				j++;
		}
		else if(i < len_val + len_rows)
		{
			A->rows[j] = (int) value;
			if(j == len_rows - 1) {
				fscanf(f, "\n");
				j = 0;
			}
			else
				j++;
		}
		else {
			A->cols[j] = (int) value;
			j++;
		}
		i++;
	}

	fclose(f);
}

void mult_mat_CSR_vect(const csr_vector_t *A, double *x, const int n)
{
	double *tmp = calloc(n, sizeof(double));

	for (int i = 0; i <= A->nb; ++i)
	{
		for (int j = A->rows[i]; j < A->rows[i+1]; ++j)
			tmp[i] += A->val[j] * x[A->cols[j]];

	}

	for (int i = 0; i < n; ++i)
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

double *PageRank(csr_vector_t *A, const double epsilon, const double beta, const int n)
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
		mult_mat_CSR_vect(A, x, n);

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
	}

	free(e);
	free(old_x);
	return x;
}

int main(int argc, char const *argv[])
{
	int n = 4;
	double beta = 0.85;
	double epsilon = 1e-4;
	csr_vector_t *matrice = malloc(sizeof(csr_vector_t*));

	read_sparse_from_file("sparse2.txt", matrice, n);

	double *res = PageRank(matrice, epsilon, beta, n);

	printf("result:\n");
	for (int i = 0; i < n; ++i)
		printf("%e ", res[i]);
	printf("\n");

	free(matrice->val);
	free(matrice->rows);
	free(matrice->cols);
	free(matrice);
	free(res);

	return 0;
}