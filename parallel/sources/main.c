#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>

int main(int argc, char **argv)
{

	double beta = 0.85;
	double epsilon = 1e-4;
	csr_vector_t *matrice = malloc(sizeof(csr_vector_t));

	if (read_sparse_from_file("data/sparse2.txt", matrice) == -1)
		return 1;

	double *res = PageRank_par(matrice, epsilon, beta, argc, argv);
	MPI_Finalize();

	printf("result:\n");
	for (int i = 0; i < matrice->nb; ++i)
		printf("%e ", res[i]);
	printf("\n");

	free(matrice->val);
	free(matrice->rows);
	free(matrice->cols);
	free(matrice);
	free(res);
}