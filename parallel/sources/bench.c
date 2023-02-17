#include <time.h>

#include "../headers/lib.h"
#include <omp.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

enum
{
    NS_PER_SECOND = 1000000000
};

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec += t2.tv_nsec - t1.tv_nsec;
    td->tv_sec += t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
}

static __inline__ unsigned long long rdtsc(void)
{
    unsigned a, d;
    asm("cpuid");
    asm volatile("rdtsc"
                 : "=a"(a), "=d"(d));

    return (((unsigned long long)a) | (((unsigned long long)d) << 32));
}

int main(int argc, char **argv)
{

    char *name_file;

    unsigned long long start_r, end_r;

    name_file = (char *)malloc(sizeof(char) * strlen("data/sparse100000.txt") + 1);
    strcpy(name_file, "data/sparse100000.txt");
    double beta = 0.85;
    double epsilon = 1e-4;
    csr_vector_t *matrice = malloc(sizeof(csr_vector_t));

    if (read_sparse_from_file(name_file, matrice) == -1)
        return 1;

    struct timespec start, finish, delta;

    delta.tv_nsec = 0;
    delta.tv_sec = 0;
    MPI_Init(&argc, &argv);

    for (int i = 0; i < 20; i++)
    {

        clock_gettime(CLOCK_REALTIME, &start);
        start_r = rdtsc();
        double *res = PageRank_par(matrice, epsilon, beta, argc, argv);
        clock_gettime(CLOCK_REALTIME, &finish);
        end_r = rdtsc()-start_r;
        sub_timespec(start, finish, &delta);
    }

    MPI_Finalize();

    printf("\tTemps = %d%.9ld\n\tRdtsc = %llu\n", (int)delta.tv_sec / 20, delta.tv_nsec /20 ,end_r/20);

    /*  printf("result:\n");
      for (int i = 0; i < 6; ++i)
          printf("%e ", res[i]);
      printf("\n");
  */
}