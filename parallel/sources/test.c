#include "../headers/lib.h"

csr_vector_t *matrice;
int n;
double beta, epsilon;

static void test_pagerank()
{
    double *res = PageRank(matrice, epsilon, beta, n);

    assert_float_equal(0.372284, res[0], 0.000001);
    assert_float_equal(0.107980, res[1], 0.000001);
    assert_float_equal(0.411755, res[2], 0.000001);
    assert_float_equal(0.107980, res[3], 0.000001);
}

int main()
{
    n = 4;
    beta = 0.85;
    epsilon = 1e-4;
    matrice = malloc(sizeof(csr_vector_t *));

    read_sparse_from_file("./data/sparse2.txt", matrice, n);

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pagerank),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}