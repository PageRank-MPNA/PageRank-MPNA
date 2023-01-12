#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ALIGN_SIZE 512
#define ASIN_LENGTH 10
#define MAX_LENGTH 233667
#define MAX_LINE 9430088


void load_asin(char **asin_tab)
{
	char *buffer = aligned_alloc(16, sizeof(char)*16);
	memset(buffer, 0, sizeof(char)*16);

	printf("Buffer (load_asin) alloué et initialisé à zéro\n");

	FILE *f = fopen("id_product_list.csv", "r");
	if(!f)
	{
		perror("id_product_list.csv");
		exit(EXIT_FAILURE);
	}

	printf("Lecture de id_product_list.csv ...\n");

	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		fgets(buffer, 16, f);
		strncpy(asin_tab[i], buffer, ASIN_LENGTH+1);
	}

	printf("Fermeture de ../correct_metadata.json ...\n");

	fclose(f);
	free(buffer);
}

void compute_hash_map(uint64_t *hash_tab, char **asin_tab)
{
	uint64_t paddr = (uint64_t) asin_tab;
	uint64_t collide = 0;

	for(uint64_t i = 0; i < MAX_LINE; i++)
	{
		uint64_t addr = (uint64_t) &asin_tab[i];
		uint64_t h = addr - paddr; // Hash Function
		if(hash_tab[h] != 0)
			collide++;
		hash_tab[h] = i;
	}

	printf("Il y a eu %ld collisions\n", collide);
}

int main(int argc, char **argv)
{
	char **asin_tab = aligned_alloc(ALIGN_SIZE, sizeof(char *) * MAX_LINE);
	for(int64_t j = 0; j < MAX_LINE; ++j)
		asin_tab[j] = aligned_alloc(ALIGN_SIZE, sizeof(char)*(ASIN_LENGTH + 1));

	printf("Tableau des id produits alloué\n");

	load_asin(asin_tab);
	printf("Tableau des id produits chargé\n");

	uint64_t hash_length = (uint64_t) &asin_tab[MAX_LINE-1] - (uint64_t) asin_tab;

	uint64_t *hash_tab = aligned_alloc(ALIGN_SIZE, sizeof(uint64_t) * hash_length);
	if(!hash_tab)
	{
		perror("hash_tab");
		exit(EXIT_FAILURE);
	}
	memset(hash_tab, 0, sizeof(uint64_t) * hash_length);

	printf("Table de hachage de taille %ld alloué\n", hash_length);

	compute_hash_map(hash_tab, asin_tab);
	printf("Table de hachage chargé\n");

	// for(int i = 0; i < 100; ++i)
	// 	printf("%s\n", asin_tab[i]);

	// FILE *f = fopen("../correct_metadata.json", "r");
	// if(!f)
	// 	return printf("Cannot open correct_metadata.json\n"), 1;

	// // uint64_t i = 0;
	// char *buffer = aligned_alloc(ALIGN_SIZE, sizeof(char)*MAX_LENGTH);
	// memset(buffer, 0, sizeof(char)*MAX_LENGTH);

	// while(!feof(f))
	// {
	// 	if(i%100000==0)
	// 	{
	// 		printf("%lu\n", i);
	// 	}

	// 	fgets(buffer, MAX_LENGTH, f);

	// 	if(ferror(f))
	// 	{
    //         fprintf(stderr, "Reading error with code %d\n", errno);
    //         break;
    //     }

    //     // result = strstr(buffer, "0006458718");
    //     // if(result != NULL)
    //     // 	printf("%c%c%c%c%c%c%c%c%c%c\n", buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

    //     i++;
	// }

	// free(buffer);
	// fclose(f);
	
	printf("Libération de la table de hachage...\n");
	free(hash_tab);

	printf("Libération du tableau des id produits...\n");
	for(uint64_t i = 0; i < MAX_LINE; ++i)
		free(asin_tab[i]);
	free(asin_tab);

	printf("Libéré\n");

	return 0;
}
