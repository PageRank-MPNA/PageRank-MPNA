#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ALIGN_SIZE 512
#define ASIN_LENGTH 10
#define MAX_LENGTH 233667
#define MAX_LINE 9430088

struct hash_item {
	uint64_t value;
	char *asin;
};

void load_asin(char **asin_tab)
{
	char *buffer = aligned_alloc(16, sizeof(char)*16);
	memset(buffer, 0, sizeof(char)*16);

	printf("Buffer (load_asin) alloué et initialisé à zéro\n");
	
	FILE *f = fopen("id_product_list.csv", "r");
	if(!f)
	{
		perror("fopen error");
		exit(EXIT_FAILURE);
	}

	printf("Lecture de id_product_list.csv ...\n");

	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		if(i%100000==0)
			printf("%lu\n", i);

		fgets(buffer, 16, f);
		strncpy(asin_tab[i], buffer, ASIN_LENGTH+1);
	}

	printf("Fermeture de ../correct_metadata.json ...\n");

	fclose(f);
	free(buffer);
}

int hash1(char *str)
{
	uint64_t capacity = 16777216; //2^24 > MAX_LINE
	uint64_t sum = 0, mul = 0;
	for(int i = 0; i < 10; ++i)
	{
		mul = ((i&3)==0) ? 1 : mul * 256;
		sum += str[i] * mul;
	}

	return sum & (capacity-1); //1981 collisions
}

int hash2(char *str)
{
	uint64_t capacity = 16777216; //2^24 > MAX_LINE
	uint64_t h = 0, g = 0;
	for(int i = 0; i < 10; ++i)
	{
		g = h & 0xf8000000;
		h <<= 5;
		h ^= g >> 27;
		h ^= str[i];
	}

	return h & (capacity-1); //404 collisions
}

// SMLRBIMX03 -> 534D4C5242494D583033

void detect_collisions(char **asin_tab)
{
	uint64_t prev = 0;
	uint64_t next = 0;
	uint64_t collide = 0;
	for(uint64_t i = 0; i < MAX_LINE; i++)
	{
		if(i%100000==0)
			printf("%lu\n", i);

		next = hash2(asin_tab[i]);
		if(prev == next)
			collide++;
		prev = next;
	}

	printf("Il y a eu %ld collisions\n", collide);
}

void create_hash_map(struct hash_item *hash_map, char **asin_tab)
{	
	uint64_t j = 0;
	for(uint64_t i = 1; i < MAX_LINE; ++i)
	{
		uint64_t k = 0;
		j = hash2(asin_tab[i]);
		if(!strcmp(hash_map[j].asin, ""))
		{
			k = 1;
			while(hash_map[k].value != 0) k++;
		}
		hash_map[j+k] = (struct hash_item) {i, asin_tab[i]};
	}
}

/*
remplacer asin_tab par le tableau de hachage, gain de place, ou pas ça peut peut-etre encore servir
rajouter le str dans item pour vérifier si à l'appel on tombe bien sur la bonne case
arriver à gérer les collisions putain
*/

int main(int argc, char **argv)
{
	char **asin_tab = aligned_alloc(ALIGN_SIZE, sizeof(char *) * MAX_LINE);
	for(int64_t j = 0; j < MAX_LINE; ++j)
		asin_tab[j] = aligned_alloc(ALIGN_SIZE, sizeof(char)*(ASIN_LENGTH + 1));

	printf("Tableau des id produits alloué\n");

	load_asin(asin_tab);
	printf("Tableau des id produits chargé\n");

	printf("Détection de collisions de hachage...\n");
	detect_collisions(asin_tab);

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

	printf("Libération du tableau des id produits...\n");
	for(uint64_t i = 0; i < MAX_LINE; ++i)
	{
		free(asin_tab[i]);
		if(i%1000000==0)
			printf("%ld\n", i);
	}
	free(asin_tab);
	printf("Libéré\n");

	return 0;
}
