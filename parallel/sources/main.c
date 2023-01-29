#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>

int main(int argc, char **argv)
{

    int n;
    double beta;
    double epsilon;
    double *res;
    csr_vector_t *matrice;

    n = 4;
    beta = 0.85;
    epsilon = 1e-4;
    matrice = malloc(sizeof(csr_vector_t *));

    read_sparse_from_file("./data/sparse2.txt", matrice, n);
    res = PageRank_par(matrice, epsilon, beta, n, argc, argv);

    free(matrice->val);
    free(matrice->rows);
    free(matrice->cols);
    free(matrice);

    printf("result:\n");
    for (int i = 0; i < n; ++i)
        printf("%lf ", res[i]);
    printf("\n");

    // free(res);
}