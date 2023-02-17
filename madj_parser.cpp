#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <time.h>

#define MAX_LINE 9430088

double elapsed(struct timespec &start, struct timespec &stop)
{
	return (double) stop.tv_sec - (double) start.tv_sec + ((double) stop.tv_nsec - (double) start.tv_nsec)*1e-9;
}

int main(int argc, char **argv)
{
	struct timespec local_start, local_stop;

	std::string buffer;
	std::vector<int> line_vec;
	line_vec.push_back(0);
	auto temp = 0;

	char seps[] = " \n";

	std::ifstream datafile("also_bought_col.txt");
    if(!datafile.is_open())
    {
        perror("also_bought_col.txt");
		exit(EXIT_FAILURE);
    }

    std::ofstream csr_col("csr_col.txt");
    if(!csr_col.is_open())
    {
        perror("csr_col.txt");
		exit(EXIT_FAILURE);
    }

    std::ofstream csr_line("csr_line.txt");
    if(!csr_line.is_open())
    {
        perror("csr_line.txt");
		exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	for(int i = 0; i < MAX_LINE; ++i)
	{
		// if(i%100000==0)
		// 	std::cout << i << std::endl;

		std::set<int> col_set;
		auto first = 1;
		std::getline(datafile, buffer);

		char *token = strtok(&buffer[0], seps);
        while(token != NULL)
        {
        	std::string tok{token};

        	if(!first)
        	{
        		col_set.insert(std::stoi(tok));
        	}
        	
        	token = strtok(NULL, seps);

        	first = 0;
        }

        for(auto it : col_set)
	        csr_col << it << "\n";

        line_vec.push_back(col_set.size());
		
		// for(auto j : col_set)
		// 	std::cout << j << " ";
		// putchar('\n');        
	}

	for(auto it = 0; it < line_vec.size(); ++it)
	{
		temp += line_vec[it];
		csr_line << temp << "\n";
	}

	std::cout << temp << " " << line_vec.size() << " " << temp << std::endl;

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	std::cout << "COmpute and write in files : 	  " << elapsed(local_start, local_stop) << " secondes\n" << std::endl;

	csr_line.close();
	csr_col.close();
	datafile.close();

	return 0;
}
