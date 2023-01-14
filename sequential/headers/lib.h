#pragma once

typedef struct csr_vector_s
{
	double *val;
	int *rows;
	int *cols;
	int nb;
} csr_vector_t;

int read_sparse_from_file(const char *filename, csr_vector_t *A, const int n);

void mult_mat_CSR_vect(const csr_vector_t *A, double *x, const int n);

void mult_mat_1D_vect(const double *A, double *x, const int n);

const double norm2(const double *x, const int n);

double *PageRank(csr_vector_t *A, const double epsilon, const double beta, const int n);