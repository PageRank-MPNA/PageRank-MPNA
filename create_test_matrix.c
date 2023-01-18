#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nnz = 0;
int row = 0;
int col = 0;

void create_matrix(int *I, int *J, double *val)
{
    int n = 0;
    while (n < nnz)
    {
        int i = rand() % row;
        int j = rand() % col;
        double v = (double)rand() / (double)(RAND_MAX);
        int k;
        for (k = 0; k < n; k++)
            if (I[k] == i && J[k] == j)
                break;
        if (k == n)
        {
            I[n] = i;
            J[n] = j;
            val[n] = v;
            n++;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: ./%s <num_nnz> <num_row> <num_col>\n You entered %d args so default values are used\n", argv[0], argc);
        nnz = 10000;
        row = 1000;
        col = 1000;
    }
    else
    {
        nnz = atoi(argv[1]);
        row = atoi(argv[2]);
        col = atoi(argv[3]);
    }
    srand(time(NULL));

    int *I = (int *)malloc(nnz * sizeof(int));
    int *J = (int *)malloc(nnz * sizeof(int));
    double *val = (double *)malloc(nnz * sizeof(double));

    create_matrix(I, J, val);

    // Save the matrix to a file in CSR format
    FILE *fp = fopen("test_matrix.csr", "w");
    if (!fp)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%d %d %d\n", nnz, row, col);

    for (int i = 0; i < nnz; i++)
    {
        fprintf(fp, "%lf %d %d \n", val[i], I[i] + 1, J[i] + 1);
    }

    fclose(fp);

    free(I);
    free(J);
    free(val);

    return 0;
}