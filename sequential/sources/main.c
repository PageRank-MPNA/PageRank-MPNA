#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "../headers/lib.h"

int main(int argc, char const *argv[])
{
	int n = 4;
	double beta = 0.85;
	double epsilon = 1e-4;
	csr_vector_t *matrice = malloc(sizeof(csr_vector_t*));

	if(read_sparse_from_file("data/sparse2.txt", matrice, n) == -1)
		return 1;

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