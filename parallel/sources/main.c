#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include <string.h>


int main(int argc, char **argv)
{

	char *name_file;

	if (argc == 2)
	{
		name_file = (char *)malloc(strlen(argv[0]) + 1);
		strcpy(name_file, argv[1]);
	}
	else
	{
		name_file = (char *)malloc(sizeof(char) * strlen("../test_matrix.csr") + 1);
		strcpy(name_file, "../test_matrix.csr");
	}

	printf("Name = %s\n", name_file);

	double beta = 0.85;
	double epsilon = 1e-4;
	csr_vector_t *matrice = malloc(sizeof(csr_vector_t));

	MPI_Init(&argc, &argv);

	if (read_sparse_from_file(name_file, matrice) == -1)
		return 1;

	double *res = PageRank_par(matrice, epsilon, beta, argc, argv);
	// double *res = PageRank(matrice, epsilon, beta);
	MPI_Finalize();

	printf("result:\n");
	for (int i = 0; i < 10; ++i)
		printf("%e ", res[i]);
	printf("\n");

	free(res);
}