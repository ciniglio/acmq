#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

// buf must be a NULL terminated string
int push(char * buf){
  int len = strnlen(buf, MAX_STRING_LENGTH);
  char * str = malloc(sizeof(char) * (len + 5 + 1));
  if(str == NULL){
    perror("alloc");
    return 1;
  }
  strlcpy(str, "PUSH ", 6);
  strlcat(str, buf, len + 5 + 1);

  if(send_data_through_connection(str) != 0){
    perror("sending");
    return 1;
  }

  free(str);

  char * res;
  if(recv_data_from_connection(&res) != 4 ||
     strncmp(res, "ACK", 3) != 0){
    perror("No ACK");
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]){
  if(init_connection(argv[1], "3443") != 0){
    exit(1);
  }
  push("this on");
  return 0;
}
