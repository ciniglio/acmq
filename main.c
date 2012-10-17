#include "mq_router.h"
#include "server.h"

int main(){
  return create_server(&handle_string, "3443");
}
