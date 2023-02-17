#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>

int read_sparse_from_file(const char *filename, csr_vector_t *A)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return perror("Failed: "), -1;

    int buf;
    fscanf(f, "%d %d %d\n", &(A->nb), &(A->dim), &buf);

    A->val = malloc(sizeof(double) * A->nb);
    A->rows = malloc(sizeof(int) * (A->dim+1));
    A->cols = malloc(sizeof(int) * A->nb);

    for (int i = 0; i < A->nb; i++)
        fscanf(f, "%lf ", &(A->val[i]));
    fscanf(f, "\n", &buf);

    for (int i = 0; i < A->dim+1; i++)
        fscanf(f, "%d ", &(A->rows[i]));
    fscanf(f, "\n", &buf);

    for (int i = 0; i < A->nb; i++)
        fscanf(f, "%d ", &(A->cols[i]));

    fclose(f);

    return 0;
}

void mult_mat_CSR_vect(const csr_vector_t *A, double *x, const int n)
{
    double *tmp = calloc(n, sizeof(double));

#pragma omp parallel for
    for (int i = 0; i < n; ++i)
        for (int j = A->rows[i]; j < A->rows[i + 1]; ++j)
            tmp[i] += A->val[j] * x[A->cols[j]];

#pragma omp parallel for
    for (int i = 0; i < n; ++i)
        x[i] = tmp[i];
    free(tmp);
}

void mult_mat_CSR_vect_par(const csr_vector_t *A, double *x, const int n, unsigned start, unsigned end)
{
    double *tmp = calloc(end-start, sizeof(double));

#pragma omp parallel for
    for (int i = start; i < end; ++i)
        for (int j = A->rows[i]; j < A->rows[i + 1] && j < n; ++j)
            tmp[i - start] += A->val[j] * x[A->cols[j]];

#pragma omp parallel for
    for (int i = start; i < end; ++i)
        x[i] = tmp[i-start];
    free(tmp);
}

const double norm2(const double *x, const int n)
{
    double res = 0.0;

#pragma omp parallel for reduction(+ \
                                   : res)
    for (int i = 0; i < n; ++i)
        res += x[i] * x[i];

    return sqrt(res);
}

double *PageRank_par(csr_vector_t *A, const double epsilon, const double beta, int argc, char **argv)
{
    int i = 0, n = A->dim;

    unsigned rank, nranks, nlocal, start, end;
    double *x = calloc(n, sizeof(double));
    double *old_x = calloc(n, sizeof(double));
    double *e = calloc(n, sizeof(double));

    const double frac = 1.0 / (double)n;
    const double cst = (1 - beta) * frac;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

#pragma omp parallel for
    for (int j = 0; j < n; ++j)
    {
        x[j] = frac;
        old_x[j] = frac;
        e[j] = cst;
    }

    // How many every rank will compute
    nlocal = n / nranks;

    // Range to start and end
    start = rank * nlocal;
    end = start + nlocal;

    if (rank == n - 1)
        end = n;

    unsigned loop = 1;

    while (loop)
    {
        // Compute multiplication matrix-vector

        mult_mat_CSR_vect_par(A, x, n, start, end);

        // Compute teleportation
        double norm1 = 0.0;

#pragma omp parallel for reduction(+ \
                                   : norm1)
        for (int j = start; j < end; ++j)
        {
            const double val = beta * x[j] + e[j];
            x[j] = val;
            // Reduce the x vector
            norm1 += val;
        }

        double global;

        // Sum the local norm
        MPI_Allreduce(&norm1, &global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        norm1 = global;

#pragma omp parallel for
        for (int j = start; j < end; ++j)
        {
            // Normalize x
            x[j] = x[j] / norm1;
            // Test break value
            old_x[j] = x[j] - old_x[j];
        }

        // Send local part of the x and old_x vector
        if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, old_x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
            printf("Erreur all gather \n");
        if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
            printf("Erreur all gather \n");

        if (norm2(old_x, n) < epsilon)
            loop = 0;

#pragma omp parallel for
        for (int j = start; j < end; ++j)
            old_x[j] = x[j];

        // Send the loop state, it should be the same for everyone
        if (MPI_Allreduce(MPI_IN_PLACE, &loop, 1, MPI_UNSIGNED, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS)
            printf("Error all reduce \n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    free(e);
    free(old_x);
    return x;
}

double *PageRank(csr_vector_t *A, const double epsilon, const double beta)
{
    int n = A->dim;

    int i = 0;
    double *x = calloc(n, sizeof(double));
    double *old_x = calloc(n, sizeof(double));
    double *e = calloc(n, sizeof(double));

    const double frac = 1.0 / (double)n;
    const double cst = (1 - beta) * frac;

// Init
#pragma omp parallel for
    for (int j = 0; j < n; ++j)
    {
        x[j] = frac;
        old_x[j] = frac;
        e[j] = cst;
    }

    int loop = 1;
    while (loop)
    {
        i++;

        // Compute multiplication matrix-vector
        mult_mat_CSR_vect(A, x, n);

        // Compute teleportations
        double norm1 = 0.0;
#pragma omp parallel for reduction(+ \
                                   : norm1)
        for (int j = 0; j < n; ++j)
        {
            const double val = beta * x[j] + e[j];
            x[j] = val;
            // Reduce the x vector
            norm1 += val;
        }

#pragma omp parallel for
        for (int j = 0; j < n; ++j)
        {
            // Normalize x
            x[j] = x[j] / norm1;
            // Test break value
            old_x[j] = x[j] - old_x[j];
        }

        if (norm2(old_x, n) < epsilon)
            break;

// Keep x vector
#pragma omp parallel for
        for (int j = 0; j < n; ++j)
            old_x[j] = x[j];
    }

    free(e);
    free(old_x);
    return x;
}