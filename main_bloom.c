#include "bloom.h"
#include "bloom_router.h"
#include "server.h"

#include <stdlib.h>

#define ERROR_RATE .05
#define SIZE 100000

int main(){
  BLOOM * bloom = bloom_create_from_file(SIZE,ERROR_RATE);
  int ret = create_server(&handle_string, "3443", bloom);
  free(bloom);
  return ret;
}
