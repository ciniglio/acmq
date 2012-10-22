#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

int pop(struct client * c, char ** res){
  if(client_transaction(c, "POP", res) != 0){
    perror("sending");
    return 1;
  }
  return 0;
}

// buf must be a NULL terminated string
int push(struct client * c, char * buf){
  int len = strnlen(buf, MAX_STRING_LENGTH);
  char * str = malloc(sizeof(char) * (len + 5 + 1));
  if(str == NULL){
    perror("alloc");
    return 1;
  }
  strlcpy(str, "PUSH ", 6);
  strlcat(str, buf, len + 5 + 1);

  if(client_transaction(c, str, NULL) != 0){
    perror("sending");
    return 1;
  }

  free(str);
  return 0;
}

int main_t(int argc, char *argv[]){
  struct client * c = initialize_client("localhost", "3443");

  push(c, "this on");

  char * res;
  pop(c, &res);
  printf("result: %s\n", res);
  free(res);
  return 0;
}
