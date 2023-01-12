#include "../headers/lib.h"
#include <omp.h>

int main()
{
    int n = 4;
    double beta = 0.85;
    double epsilon = 1e-4;
    csr_vector_t *matrice = malloc(sizeof(csr_vector_t *));


    read_sparse_from_file("./data/sparse2.txt", matrice, n);

    double *res = PageRank(matrice, epsilon, beta, n);

    printf("result:\n");
    for (int i = 0; i < n; ++i)
        printf("%lf ", res[i]);
    printf("\n");

    free(matrice->val);
    free(matrice->rows);
    free(matrice->cols);
    free(matrice);
    free(res);

 
}