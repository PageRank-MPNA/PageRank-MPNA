#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define ALIGN_SIZE 512
#define ASIN_LENGTH 10
#define MAX_LENGTH 233667
#define MAX_LINE 9430088


void load_asin(char **asin_tab)
{
	char *buffer = aligned_alloc(16, sizeof(char)*16);
	memset(buffer, 0, sizeof(char)*16);

	FILE *f = fopen("id_product_list.csv", "r");
	if(!f)
	{
		perror("id_product_list.csv");
		exit(EXIT_FAILURE);
	}

	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		fgets(buffer, 16, f);
		strncpy(asin_tab[i], buffer, ASIN_LENGTH+1);
	}

	fclose(f);
	free(buffer);
}

void compute_hash_map(uint64_t *hash_tab, char **asin_tab)
{
	uint64_t paddr = (uint64_t) asin_tab;
	// uint64_t collide = 0;

	for(uint64_t i = 0; i < MAX_LINE; i++)
	{
		uint64_t addr = (uint64_t) &asin_tab[i];
		uint64_t h = addr - paddr; // Hash Function
		// if(hash_tab[h] != 0)
		// 	collide++;
		hash_tab[h] = i;
	}

	// printf("Il y a eu %ld collisions\n", collide);
}

double elapsed(struct timespec start, struct timespec stop)
{
	return (double) stop.tv_sec - (double) start.tv_sec + ((double) stop.tv_nsec - (double) start.tv_nsec)*1e-9;
}

int main(int argc, char **argv)
{
	struct timespec global_start, global_stop;
	struct timespec local_start, local_stop;

	clock_gettime(CLOCK_MONOTONIC_RAW, &global_start);

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	char **asin_tab = aligned_alloc(ALIGN_SIZE, sizeof(char *) * MAX_LINE);
	for(int64_t j = 0; j < MAX_LINE; ++j)
		asin_tab[j] = aligned_alloc(ALIGN_SIZE, sizeof(char)*(ASIN_LENGTH + 1));
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);

	printf("Allocation product id array : 	  %lf secondes\n", elapsed(local_start, local_stop));

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	load_asin(asin_tab);
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);

	printf("Fill product id array : 	  %lf secondes\n", elapsed(local_start, local_stop));

	uint64_t hash_length = (uint64_t) &asin_tab[MAX_LINE-1] - (uint64_t) asin_tab;

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	uint64_t *hash_tab = aligned_alloc(ALIGN_SIZE, sizeof(uint64_t) * hash_length);
	if(!hash_tab)
	{
		perror("hash_tab");
		exit(EXIT_FAILURE);
	}
	memset(hash_tab, 0, sizeof(uint64_t) * hash_length);
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);

	printf("Allocation and init hash table :  %lf secondes\n", elapsed(local_start, local_stop));

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	compute_hash_map(hash_tab, asin_tab);
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	
	printf("Fill hash table : 		  %lf secondes\n", elapsed(local_start, local_stop));

	FILE *datafile = fopen("correct_metadata.json", "r");
	if(!datafile)
	{
		perror("correct_metadata.json");
		exit(EXIT_FAILURE);
	}

	char *buffer = aligned_alloc(ALIGN_SIZE, sizeof(char)*MAX_LENGTH);
	memset(buffer, 0, sizeof(char)*MAX_LENGTH);

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		// if(i%100000==0)
		// {
		// 	printf("%lu\n", i);
		// }

		fgets(buffer, MAX_LENGTH, datafile);

		if(ferror(datafile))
		{
            perror("Read correct_metadata.json");
            exit(EXIT_FAILURE);
        }

        // result = strstr(buffer, "0006458718");
        // if(result != NULL)
        // 	printf("%c%c%c%c%c%c%c%c%c%c\n", buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	printf("Read and store in buffer : 	  %lf secondes\n", elapsed(local_start, local_stop));

	free(buffer);
	fclose(datafile);

	free(hash_tab);

	clock_gettime(CLOCK_MONOTONIC_RAW, &local_start);
	for(uint64_t i = 0; i < MAX_LINE; ++i)
		free(asin_tab[i]);
	free(asin_tab);
	clock_gettime(CLOCK_MONOTONIC_RAW, &local_stop);
	printf("Free product id array : 	  %lf secondes\n", elapsed(local_start, local_stop));

	clock_gettime(CLOCK_MONOTONIC_RAW, &global_stop);

	printf("------------------------------------------\n");
	printf("Total elapsed time : 		  %lf secondes\n", elapsed(global_start, global_stop));

	return 0;
}
