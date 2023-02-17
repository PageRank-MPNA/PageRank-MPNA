#include "../headers/lib.h"
#include <mpi.h>
#include <cmocka.h>

csr_vector_t *matrice;
int n, argc_g;
char **argv_g;
double beta, epsilon;

static void test_mult_mat_CSR_vect()
{

    // MPI_Finalize();

    double *x = malloc(sizeof(double) * n);
    unsigned rank, nranks, nlocal, start, end;

    //  MPI_Init(&argc_g, &argv_g);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

    nlocal = n / nranks;

    start = rank * nlocal;
    end = start + nlocal;

    for (int i = 0; i < n; ++i)
        x[i] = 0.0;
    mult_mat_CSR_vect_par(matrice, x, n, start, end);

    if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
        printf("Erreur all gather \n");

    if (rank == 0)
        for (int i = 0; i < n; ++i)
        {
            assert_float_equal(0.000000, x[i], 1e-6);
        }

    for (int i = 0; i < n; ++i)
        x[i] = -1.0;
    mult_mat_CSR_vect_par(matrice, x, n, start, end);
    if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
        printf("Erreur all gather \n");

    double res[6] = {-1.000000, -0.000000, -0.333333, -0.000000, -1.000000, -0.000000};

    if (rank == 0)
        for (int i = start; i < end; ++i)
        {
            assert_float_equal(res[i], x[i], 1e-6);
        }
    for (int i = 0; i < n; ++i)
        x[i] = 2.0;
    mult_mat_CSR_vect_par(matrice, x, n, start, end);
    if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
        printf("Erreur all gather \n");

    double res2[6] = {2.000000, 0.000000, 0.666667, 0.000000, 2.000000, 0.000000};

    if (rank == 0)
        for (int i = start; i < end; ++i)
        {
            assert_float_equal(res2[i], x[i], 1e-6);
        }

    free(matrice->val);
    free(matrice->rows);
    free(matrice->cols);
    free(matrice);
    free(x);

    MPI_Barrier(MPI_COMM_WORLD);
}

static void test_pagerank()
{
    int rank, nranks, nlocal;
    n = matrice->nb;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

    nlocal = n / nranks;

    double *res = PageRank_par(matrice, epsilon, beta, argc_g, argv_g);
    if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, res, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
        printf("Erreur all gather \n");
    if (rank == 0)
    {
        assert_float_equal(0.185018, res[0], 1e-6);
        assert_float_equal(0.060508, res[1], 1e-6);
        assert_float_equal(0.187391, res[2], 1e-6);
        assert_float_equal(0.060508, res[3], 1e-6);
        assert_float_equal(0.446065, res[4], 1e-6);
        assert_float_equal(0.060508, res[5], 1e-6);
    }
}

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);

    argc_g = argc;
    argv_g = argv;

    beta = 0.85;
    epsilon = 1e-4;
    matrice = malloc(sizeof(csr_vector_t));

    read_sparse_from_file("./data/sparse2.txt", matrice);

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pagerank),
        cmocka_unit_test(test_mult_mat_CSR_vect),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}