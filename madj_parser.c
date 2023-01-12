#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ALIGN_SIZE 512
#define ASIN_LENGTH 10
#define MAX_LENGTH 233667
#define MAX_LINE 9430088
#define MAX_HASH 16777216 //2^24 > MAX_LINE

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
		// if(i%100000==0)
		// 	printf("%lu\n", i);

		fgets(buffer, 16, f);
		strncpy(asin_tab[i], buffer, ASIN_LENGTH+1);
	}

	printf("Fermeture de ../correct_metadata.json ...\n");

	fclose(f);
	free(buffer);
}

int hash(char *str)
{
	int a = 0x9e3779b9;
	int b = 0x9e3779b9;
	int c = 10;
	
   	a += str[0] + (str[1] << 8) + (str[2] << 16) + (str[3] << 24);
   	b += str[4] + (str[5] << 8) + (str[6] << 16) + (str[7] << 24);
   	c += (str[8] << 8) + (str[9] << 16);

   	a -= b; a -= c; a ^= c >> 13;
	b -= c; b -= a; b ^= a << 8;
	c -= a; c -= b; c ^= b >> 13;
  	a -= b; a -= c; a ^= c >> 12;
  	b -= c; b -= a; b ^= a << 16;
  	c -= a; c -= b; c ^= b >> 5;
 	a -= b; a -= c; a ^= c >> 3;
  	b -= c; b -= a; b ^= a << 10;
  	c -= a; c -= b; c ^= b >> 15;

   	return c & (MAX_HASH-1);
}

// SMLRBIMX03 -> 534D4C5242494D583033

void detect_collisions(char **asin_tab)
{
	// FILE *fw = fopen("output_hash", "w");
	// if(!fw)
	// {
	// 	perror("output_hash");
	// 	exit(EXIT_FAILURE);
	// }

	uint64_t prev = 0;
	uint64_t next = 0;
	uint64_t collide = 0;
	for(uint64_t i = 0; i < MAX_LINE; i++)
	{
		// if(i%100000==0)
		// 	printf("%lu\n", i);

		next = hash(asin_tab[i]);
		// fprintf(fw ,"%ld\n", next);
		if(prev == next)
			collide++;
		prev = next;
	}

	// fclose(fw);

	printf("Il y a eu %ld collisions\n", collide);
}

/*
remplacer asin_tab par le tableau de hachage, gain de place, ou pas ça peut peut-etre encore servir
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
		// if(i%1000000==0)
		// 	printf("%ld\n", i);
	}
	free(asin_tab);
	printf("Libéré\n");

	return 0;
}
