#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <time.h>

#define SIZE_COL 105891595
#define SIZE_LINE 9430089

double elapsed(struct timespec &start, struct timespec &stop)
{
	return (double) stop.tv_sec - (double) start.tv_sec + ((double) stop.tv_nsec - (double) start.tv_nsec)*1e-9;
}

int main(int argc, char **argv)
{
	std::array<int, SIZE_COL> array_csr_col;
	std::array<int, SIZE_LINE> array_csr_line;
	std::array<int, SIZE_LINE> array_csc_col;
	std::array<int, SIZE_COL> array_csc_line;
	std::array<int, SIZE_LINE-1> curr;

	struct timespec local_start, local_stop;

	std::ifstream csr_col("csr_col.txt");
    if(!csr_col.is_open())
    {
        perror("csr_col.txt");
		exit(EXIT_FAILURE);
    }

    std::ifstream csr_line("csr_line.txt");
    if(!csr_line.is_open())
    {
        perror("csr_line.txt");
		exit(EXIT_FAILURE);
    }

    std::ofstream csc_col("csc_col.txt");
    if(!csc_col.is_open())
    {
        perror("csc_col.txt");
		exit(EXIT_FAILURE);
    }

    std::ofstream csc_line("csc_line.txt");
    if(!csc_line.is_open())
    {
        perror("csc_line.txt");
		exit(EXIT_FAILURE);
    }

    for(auto i = 0; i < SIZE_COL; ++i)
    	csr_col >> array_csr_col[i];

    for(auto i = 0; i < SIZE_LINE; ++i)
    	csr_line >> array_csr_line[i];

    /*################################*/

    clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);

    for(auto i = 0; i < SIZE_LINE-1; ++i)
    	for(auto j = array_csr_line[i]; j < array_csr_line[i+1]; ++j)
    		array_csc_col[array_csr_col[j] + 1]++;

    for(auto i = 1; i < SIZE_LINE; ++i)
    	array_csc_col[i] += array_csc_col[i-1];

    for(auto i = 0; i < SIZE_LINE-1; ++i)
    	for(auto j = array_csr_line[i]; j < array_csr_line[i+1]; ++j)
    	{
    		int loc = array_csc_col[array_csr_col[j]] + curr[array_csr_col[j]]++;
    		array_csc_line[loc] = i;
    	}

    clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
    std::cout << "Compute transposition of csr format (csc) : 	  " << elapsed(local_start, local_stop) << " secondes\n" << std::endl;

    /*################################*/

    clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
    for(auto it : array_csc_col)
    	csc_col << it << std::endl;

    for(auto it : array_csc_line)
    	csc_line << it << std::endl;
    clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
    std::cout << "Write in file : 	  " << elapsed(local_start, local_stop) << " secondes\n" << std::endl;

    /*################################*/

    csc_line.close();
	csc_col.close();
	csr_line.close();
	csr_col.close();

	return 0;
}
