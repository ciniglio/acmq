#include "queue.h"
#include "mq_router.h"
#include "server.h"

#include <stdlib.h>

int main(){
  struct Queue * q = initialize_queue();
  int ret = create_server(&handle_string, "3443", q);
  free(q);
  return ret;
}
