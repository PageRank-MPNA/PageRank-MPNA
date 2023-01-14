#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <time.h>

#define ALIGN_SIZE 512
#define ASIN_LENGTH 10
#define MAX_LENGTH 233667
#define MAX_LINE 9430088
#define MAX_HASH 16777216

void load_asin(std::unordered_map<std::string, uint64_t> &hash_map)
{
	std::string buffer{};

	std::ifstream f("id_product_list.csv");
	if(!f.is_open())
	{
		perror("id_product_list.csv");
		exit(EXIT_FAILURE);
	}

	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		std::getline(f, buffer);
		hash_map[buffer] = i;
	}

	f.close();
}

double elapsed(struct timespec &start, struct timespec &stop)
{
	return (double) stop.tv_sec - (double) start.tv_sec + ((double) stop.tv_nsec - (double) start.tv_nsec)*1e-9;
}

int main(int argc, char **argv)
{
	struct timespec global_start, global_stop;
	struct timespec local_start, local_stop;

	std::unordered_map<std::string, uint64_t> hash_map;
	std::string buffer;
	std::string id;

	char seps[] = " :,[]{\"";
	long j = 0;

	clock_gettime(CLOCK_MONOTONIC_RAW, &global_start);

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	load_asin(hash_map);
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	std::cout << "Compute hash table : 	  " << elapsed(local_start, local_stop) << " secondes\n" << std::endl;

	std::ifstream datafile("correct_metadata.json");
    if(!datafile.is_open())
    {
        perror("correct_metadata.json");
		exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		if(i%100000==0)
			std::cout << i << std::endl;
		
		std::getline(datafile, buffer);

		std::map<long, long> col;
        std::size_t pos = buffer.find("\"related\": {");
        if(pos != buffer.npos)
        {
	        pos += 12;

	        std::string str{buffer.substr(pos, buffer.npos)};
	        char *token = strtok(&str[0], seps);
	        while(token != NULL)
	        {
	        	std::string tok{token};
	        	if(!tok.compare(0, 1, "}"))
	        		break;

	        	if(tok.compare(0, 11, "also_bought") && tok.compare(0, 11, "also_viewed") && tok.compare(0, 17, "buy_after_viewing") && tok.compare(0, 15, "bought_together"))
	    		{
	    			j = hash_map[tok];
	    			if(j)
	    				col[j] = 1;
	    		}

	        	token = strtok(NULL, seps);
	        }

	        // for(auto const &pair : col)
	        // 	std::cout << pair.first << " ";
	    	// std::cout << std::endl;
        }
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	std::cout << "Read and store in buffer : 	  " << elapsed(local_start, local_stop) << " secondes\n" << std::endl;

	datafile.close();

    clock_gettime(CLOCK_MONOTONIC_RAW, &global_stop);

	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Total elapsed time : 		  " << elapsed(global_start, global_stop) << " secondes\n\n" << std::endl;


	return 0;
}
