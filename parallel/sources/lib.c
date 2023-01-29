#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>
/*
struct Matrix* to_CSB(double **A, int size, int process)
{
    struct Matrix* matrix = malloc(sizeof(matrix));

    (size%process) ? matrix->size_block = sqrt(size) : matrix->size_block = process;

    int row = 1;

    struct CSB* block = malloc(sizeof(block));

    matrix->first = block;

    for(int i = 0; i < size; i ++)
    {
        for(int j = 0; j < size ; j ++)
        {
            if(A[i][j])
            {
               if(!row)
               {
                    struct CSB* block_next = malloc(sizeof(block));
                    block->next = block_next;
               }

            }
        }
    }


    return matrix;
}

*/
/*
int main()
{

}*/

int read_sparse_from_file(const char *filename, csr_vector_t *A, const int n)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return perror("Failed: "), -1;
    int len_val, len_rows, len_cols;

    fscanf(f, "%d %d %d\n", &len_val, &len_rows, &len_cols);

    A->nb = len_val;
    A->val = malloc(sizeof(double) * len_val);
    A->rows = malloc(sizeof(double) * len_rows);
    A->cols = malloc(sizeof(double) * len_cols);

    fseek(f, sizeof(int) + 1, SEEK_SET);

    int i = 0, j = 0;
    double value;
    while (fscanf(f, "%lf ", &value) != EOF)
    {
        if (i < len_val)
        {
            A->val[j] = value;
            if (j == len_val - 1)
            {
                fscanf(f, "\n");
                j = 0;
            }
            else
                j++;
        }
        else if (i < len_val + len_rows)
        {
            A->rows[j] = (int)value;
            if (j == len_rows - 1)
            {
                fscanf(f, "\n");
                j = 0;
            }
            else
                j++;
        }
        else
        {
            A->cols[j] = (int)value;
            j++;
        }
        i++;
    }

    // fclose(f);

    return 0;
}

void mult_mat_CSR_vect(const csr_vector_t *A, double *x, const int n)
{
    double *tmp = calloc(n, sizeof(double));

#pragma omp parallel for
    for (int i = 0; i <= A->nb; ++i)
    {
        for (int j = A->rows[i]; j < A->rows[i + 1]; ++j)
            tmp[i] += A->val[j] * x[A->cols[j]];
    }

#pragma omp parallel for
    for (int i = 0; i < n; ++i)
        x[i] = tmp[i];
    free(tmp);
}

void mult_mat_CSR_vect_par(const csr_vector_t *A, double *x, const int n, unsigned start, unsigned end)
{
    double *tmp = calloc(n, sizeof(double));

    for (int i = start; i <= end; ++i)
    {
        for (int j = A->rows[i]; j < A->rows[i + 1]; ++j)
            tmp[i] += A->val[j] * x[A->cols[j]];
    }

    for (int i = start; i < end; ++i)
        x[i] = tmp[i];
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

double *PageRank_par(csr_vector_t *A, const double epsilon, const double beta, const int n, int argc, char **argv)
{
    unsigned rank, nranks, nlocal, start, end;
    double *x = calloc(n, sizeof(double));
    double *old_x = calloc(n, sizeof(double));
    double *e = calloc(n, sizeof(double));

    const double frac = 1.0 / (double)n;
    const double cst = (1 - beta) * frac;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    for (int j = 0; j < n; ++j)
    {
        x[j] = frac;
        old_x[j] = frac;
        e[j] = cst;
    }
    nlocal = n / nranks;

    start = rank * nlocal;
    end = start + nlocal;

    if (rank == n - 1)
        end = n;

    unsigned loop = 1;

    while (loop)
    {
        mult_mat_CSR_vect_par(A, x, n, start, end);

        if (MPI_Allgather(MPI_IN_PLACE, nlocal, MPI_DOUBLE, x, nlocal, MPI_DOUBLE, MPI_COMM_WORLD) != MPI_SUCCESS)
            printf("Erreur all gather \n");

        double norm1 = 0.0;
        for (int j = 0; j < n; ++j)
        {
            const double val = beta * x[j] + e[j];
            x[j] = val;
            // Reduce the x vector
            norm1 += val;
        }
        for (int j = 0; j < n; ++j)
        {
            // Normalize x
            x[j] = x[j] / norm1;
            // Test break value
            old_x[j] = x[j] - old_x[j];
        }

        if (norm2(old_x, n) < epsilon)
            loop = 0;
        for (int j = 0; j < n; ++j)
            old_x[j] = x[j];

        if (MPI_Allreduce(MPI_IN_PLACE, &loop, 1, MPI_UNSIGNED, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS)
            printf("Error all reduce \n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        MPI_Finalize();

    return x;
}

double *PageRank(csr_vector_t *A, const double epsilon, const double beta, const int n)
{
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