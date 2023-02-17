#include <time.h>

#include "../headers/lib.h"
#include <omp.h>
#include <sys/time.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
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
    unsigned long long start_r, end_r;
    double beta = 0.85;
    double epsilon = 1e-4;
    csr_vector_t *matrice = malloc(sizeof(csr_vector_t));
    double start = 0.0, finish = 0.0, sum = 0.0;

    DIR *dir;
    struct dirent *ent;
    char *ext = ".csr";
    char path[100];

    if (argc != 2) {
        printf("Veuillez préciser le nom du répertoire.\n");
        return 1;
    }

    if ((dir = opendir(argv[1])) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG && strcmp(ext, strrchr(ent->d_name, '.')) == 0) {
                snprintf(path, sizeof(path), "%s/%s", argv[1], ent->d_name);
                
                if (read_sparse_from_file(path, matrice) == -1)
                    return 1;

                for (int i = 0; i < 20; i++)
                {
                    start   = omp_get_wtime();
                    start_r = rdtsc();
                    double *res = PageRank(matrice, epsilon, beta);
                    finish = omp_get_wtime();
                    end_r  = rdtsc() - start_r;
                    sum   += finish - start;
                }
                printf("%s :\n\tTemps = %e\n\tRdtsc = %llu\n", path, sum/20.0, end_r/20);

            }
        }
        closedir(dir);
    } else {
        printf("Impossible d'ouvrir le répertoire.\n");
        return 1;
    }
}
