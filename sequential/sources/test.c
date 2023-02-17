#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "../headers/lib.h"

static void test_mult_mat_CSR_vect()
{
    csr_vector_t *matrice = malloc(sizeof(csr_vector_t));
    read_sparse_from_file("./data/sparse2.txt", matrice);
    const int N = matrice->dim;
    double *x = malloc(sizeof(double) * N);

    for (int i = 0; i < N; ++i) x[i] = 0.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(0.000000e+00, x[0], 1e-6);
    assert_float_equal(0.000000e+00, x[1], 1e-6);
    assert_float_equal(0.000000e+00, x[2], 1e-6);
    assert_float_equal(0.000000e+00, x[3], 1e-6);

    for (int i = 0; i < N; ++i) x[i] = -1.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(-1.000000e+00, x[0], 1e-6);
    assert_float_equal(-1.333333e+00, x[1], 1e-6);
    assert_float_equal(-1.333333e+00, x[2], 1e-6);
    assert_float_equal(-0.333333e+00, x[3], 1e-6);

    for (int i = 0; i < N; ++i) x[i] = 2.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(2.000000e+00, x[0], 1e-6);
    assert_float_equal(2.666667e+00, x[1], 1e-6);
    assert_float_equal(2.666667e+00, x[2], 1e-6);
    assert_float_equal(0.666667e+00, x[3], 1e-6);

    free(matrice->val);
    free(matrice->rows);
    free(matrice->cols);
    free(matrice);
    free(x);
    
}

static void test_pagerank()
{
    double beta = 0.85;
    double epsilon = 1e-4;
    csr_vector_t *matrice = malloc(sizeof(csr_vector_t));

    if(read_sparse_from_file("data/sparse2.txt", matrice) == -1)
        return ;

    double *res = PageRank(matrice, epsilon, beta);
    assert_float_equal(0.091414e+00, res[0], 1e-6);
    assert_float_equal(0.422593e+00, res[1], 1e-6);
    assert_float_equal(0.422593e+00, res[2], 1e-6);
    assert_float_equal(0.063401e+00, res[3], 1e-6);

    free(matrice->val);
    free(matrice->rows);
    free(matrice->cols);
    free(matrice);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mult_mat_CSR_vect),
        cmocka_unit_test(test_pagerank),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}