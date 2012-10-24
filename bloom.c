#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "bloom.h"
#include "murmur.h"
#include "bloom_io.h"

#define BLOOMIN_VERSION "0.0.1"

#define SALT_CONSTANT 0x97c29b3a

/*
#define ERROR_RATE .01
#define SIZE 10000000
*/


BLOOM *bloom_init(uint32_t size, double error_rate)
{
	BLOOM *bloom;
	if ((bloom = (BLOOM *)malloc(sizeof(BLOOM))) == NULL) {
		fprintf(stderr, "Error, could not alloc a new bloom filter\n");
        return NULL;
    }
	bloom->filter = NULL;
	bloom->size = size;
	bloom->num_funcs = (uint32_t) ceil(log(1 / error_rate) / log(2));
	bloom->hashes = calloc(bloom->num_funcs, sizeof(uint32_t));
	return bloom;
}


BLOOM *bloom_load(uint32_t size, double error_rate)
{

	BLOOM *bloom = bloom_init(size, error_rate);
	if (bloom_read(bloom) == 0)
	{
		fprintf(stderr, "Error, could not alloc a new bloom filter\n");
		return NULL;
	}
	return bloom;
}


BLOOM *bloom_create(uint32_t size, double error_rate)
{
	BLOOM *bloom;

	if ((bloom = (BLOOM *)malloc(sizeof(BLOOM))) == NULL) {
        fprintf(stderr, "Error, could not alloc a new bloom filter\n");
        return NULL;
    }

	if ((bloom->filter = (uint32_t *)calloc(size, sizeof(uint32_t))) == NULL){
		return NULL;
	}
		
	bloom->num_funcs = (uint32_t) ceil(log(1 / error_rate) / log(2));
	bloom->hashes = calloc(bloom->num_funcs, sizeof(uint32_t));
	bloom->size = size;
	
	return bloom;
}


BLOOM *bloom_create_from_file(uint32_t size, double error_rate)
{
	BLOOM *bloom = bloom_load(size, error_rate); 
	return bloom;
}


static void hash_func(BLOOM *bloom, const char *key, uint32_t key_len, uint32_t *hashes)
{
	int i;
	uint32_t checksum[4];
	MurmurHash3_x64_128(key, key_len, SALT_CONSTANT, checksum);
	uint32_t h1 = checksum[0];
    uint32_t h2 = checksum[1];

    for (i = 0; i < bloom->num_funcs; i++) {
    	hashes[i] = (h1 + i * h2) % bloom->size;
    }
}

int bloom_insert(BLOOM *bloom, const char *s, uint32_t len)
{
	uint32_t i;
	uint32_t idx;

	uint32_t *hashes = bloom->hashes;
	hash_func(bloom, s, len, hashes);

	for (i = 0; i < bloom->num_funcs; ++i)
	{
		idx = hashes[i];
		bloom->filter[idx]++;
	}
	return 1;
}

int bloom_check(BLOOM *bloom, const char *s, uint32_t len)
{
	
	uint32_t i;
	uint32_t idx;

	uint32_t *hashes = bloom->hashes;
	hash_func(bloom, s, len, hashes);
	
	for (i = 0; i < bloom->num_funcs; ++i)
	{
		idx = hashes[i];
		if (bloom->filter[idx] <= 0)
		{
			return 0;
		}
	}
	return 1;
}

int bloom_delete(BLOOM *bloom, const char *s, uint32_t len)
{
	uint32_t i;
	uint32_t idx;

	uint32_t *hashes = bloom->hashes;
	hash_func(bloom, s, len, hashes);

	if (!bloom_check(bloom,s,len))
	{
		return 0;
	}
	for (i = 0; i < bloom->num_funcs; ++i)
	{
		idx = hashes[i];
		bloom->filter[idx]--;
		if (bloom->filter[idx] <= 0)
		{
			bloom->filter[idx] = 0;
		}
	}
	return 1;
}

int bloom_destroy(BLOOM *bloom)
{	
	if (bloom != NULL)

	free(bloom->filter);
	free(bloom->hashes);
	free((void *)bloom);

	return 0;

}


/*
int main()
{

	BLOOM *bloom = bloom_create_from_file(SIZE,ERROR_RATE);

	bloom_check(bloom, "newval", strlen("newval"));
	bloom_insert(bloom, "newval", strlen("newval"));
	bloom_write(bloom);
	BLOOM *bloom2 = bloom_load(SIZE,ERROR_RATE);
	bloom_check(bloom2, "newval", strlen("newval"));
	return 0;
}
*/







