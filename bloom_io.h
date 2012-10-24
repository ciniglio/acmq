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

int bloom_read(BLOOM *bloom);
int bloom_write(BLOOM *bloom);