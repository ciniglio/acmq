#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

#define PUSH "PUSH "
#define PUSHLEN 5

#define POP "POP\n"

#define EOFLEN 1

#define NEWLINE "\n"
#define NEWLINELEN 1

int pop(struct client * c, char ** res){
  if(client_transaction(c, POP, res) != 0){
    perror("sending");
    return 1;
  }
  return 0;
}

// buf must be a NULL terminated string
int push(struct client * c, char * buf){
  int len = strnlen(buf, MAX_STRING_LENGTH);
  char * str = malloc(sizeof(char) * (len + PUSHLEN + NEWLINELEN + EOFLEN));
  if(str == NULL){
    perror("alloc");
    return 1;
  }
  strlcpy(str, PUSH, PUSHLEN + EOFLEN);
  strlcat(str, buf, len + PUSHLEN + EOFLEN);
  strlcat(str, NEWLINE, len + PUSHLEN + NEWLINELEN + EOFLEN);

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
