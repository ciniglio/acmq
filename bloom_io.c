#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "persistence.h"
#include "bloom.h"
#include "murmur.h"
#include <math.h>

#define FILENAME "bloom.bin"

#define ERROR_RATE .01
#define SIZE 10000000

int bloom_read(BLOOM *bloom)
{	
	char * str;
	int size; 
	if ((size = readfile(FILENAME, &str)) == -1)
	{
		perror("file read error");
	};
	
	if (size % sizeof(uint32_t) != 0)
	{
		perror("wrong filter type size");
	}

	bloom->size = size / sizeof(uint32_t);
	bloom->filter = (uint32_t *) str;
	return 1;
}

int bloom_write(BLOOM *bloom)
{
	char *buf;
	buf = (char *)bloom->filter;
	if (writefile(FILENAME, buf, sizeof(uint32_t) * bloom->size) != 0)
	{
		perror("write error");
	}
	return 0;
}






