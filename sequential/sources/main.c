#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "../headers/lib.h"

int main(int argc, char const *argv[])
{
	double beta = 0.85;
	double epsilon = 1e-4;
	csr_vector_t *matrice = malloc(sizeof(csr_vector_t));

	if(read_sparse_from_file("../test_matrix.csr", matrice) == -1)
		return 1;

	double *res = PageRank(matrice, epsilon, beta);

	printf("result:\n");
	for (int i = 0; i < matrice->dim; ++i)
		printf("%e ", res[i]);
	printf("\n");

	free(matrice->val);
	free(matrice->rows);
	free(matrice->cols);
	free(matrice);
	free(res);

	return 0;
}