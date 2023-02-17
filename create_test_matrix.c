#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nnz = 0; // taille = nombre de valeurs non-nulles
int row = 0; // taille = nombre de ligne de la matrice dense + 1
int col = 0; // taille = nombre de valeurs non-nulles

// Pour qsort()
int intComparator(const void *first, const void *second)
{
    int firstInt = * (const int *) first;
    int secondInt = * (const int *) second;
    return firstInt - secondInt;
}

void create_matrix(int *I, int *J, double *val)
{
    // int n = 0;
    // while (n < nnz)
    // {
    //     int i = rand() % row;
    //     int j = rand() % col;
    //     double v = (double)rand() / (double)(RAND_MAX);
    //     int k;
    //     for (k = 0; k < n; k++)
    //         if (I[k] == i && J[k] == j)
    //             break;
    //     if (k == n)
    //     {
    //         I[n] = i;
    //         J[n] = j;
    //         val[n] = v;
    //         n++;
    //     }
    // }

    for(int i = 0; i < nnz; ++i)
    {
        val[i] = 1;
        J[i] = rand() % row;
    }

    I[0] = 0;
    for(int i = 1; i < row + 1; ++i)
    {
        I[i] = rand() % (nnz);
    }
    I[row] = nnz;

    qsort(I, row, sizeof(int), intComparator);

    int *temp = malloc(sizeof(int) * nnz);
    for(int i = 0; i < row; ++i)
    {
        const int ecart = I[i+1] - I[i];
        if(ecart)
        {
            for(int j = I[i]; j < I[i+1]; ++j)
                temp[j - I[i]] = J[j];

            for(int i = 0; i < ecart; ++i)
                for(int j = i+1; j < ecart; ++j)
                    if(temp[i] == temp[j])
                        temp[j] = (temp[j]+1)%ecart;

            qsort(temp, ecart, sizeof(int), intComparator);

            for(int j = I[i]; j < I[i+1]; ++j)
            {
                J[j] = temp[j - I[i]];
                val[j] /= ecart;
            }
        }
    }
    free(temp);

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

    int *I = (int *)malloc((row+1) * sizeof(int));
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
        fprintf(fp, "%lf ", val[i]);
    }
    fprintf(fp, "\n");

    for (int i = 0; i < row+1; i++)
    {
        fprintf(fp, "%d ", I[i]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < nnz; i++)
    {
        fprintf(fp, "%d ", J[i]);
    }
    fprintf(fp, "\n");

    fclose(fp);

    free(I);
    free(J);
    free(val);

    return 0;
}