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
    const int N = matrice->nb;
    double *x = malloc(sizeof(double) * N);

    for (int i = 0; i < N; ++i) x[i] = 0.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(0.000000e+00, x[0], 1e-6);
    assert_float_equal(0.000000e+00, x[1], 1e-6);
    assert_float_equal(0.000000e+00, x[2], 1e-6);
    assert_float_equal(0.000000e+00, x[3], 1e-6);
    assert_float_equal(0.000000e+00, x[4], 1e-6);
    assert_float_equal(0.000000e+00, x[5], 1e-6);

    for (int i = 0; i < N; ++i) x[i] = -1.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(-1.000000e+00, x[0], 1e-6);
    assert_float_equal(-1.333333e+00, x[1], 1e-6);
    assert_float_equal(-1.444444e+00, x[2], 1e-6);
    assert_float_equal(-3.333333e-01, x[3], 1e-6);
    assert_float_equal(0.000000e+00, x[4], 1e-6);
    assert_float_equal(0.000000e+00, x[5], 1e-6);

    for (int i = 0; i < N; ++i) x[i] = 2.0;
    mult_mat_CSR_vect(matrice, x);
    assert_float_equal(2.000000e+00, x[0], 1e-6);
    assert_float_equal(2.666667e+00, x[1], 1e-6);
    assert_float_equal(2.888889e+00, x[2], 1e-6);
    assert_float_equal(6.666666e-01, x[3], 1e-6);
    assert_float_equal(0.000000e+00, x[4], 1e-6);
    assert_float_equal(0.000000e+00, x[5], 1e-6);

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
    assert_float_equal(2.220840e-01, res[0], 1e-6);
    assert_float_equal(3.159032e-01, res[1], 1e-6);
    assert_float_equal(3.187280e-01, res[2], 1e-6);
    assert_float_equal(9.134221e-02, res[3], 1e-6);
    assert_float_equal(2.597132e-02, res[4], 1e-6);
    assert_float_equal(2.597132e-02, res[5], 1e-6);

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