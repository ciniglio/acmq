#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

int pop(char ** res){
  if(send_data_through_connection("POP") != 0){
    perror("sending");
    return 1;
  }
  char * long_res;

  if (recv_data_from_connection(&long_res) < 0){
    perror("Recieving");
    return 1;
  }

  int len = strlen(long_res);
  *res = malloc(sizeof(char) * (len + 1));
  strlcpy(*res, long_res, len + 1);
  free(long_res);

  return 0;
}

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
  destroy_connection();


  if(init_connection(argv[1], "3443") != 0){
    exit(1);
  }
  char * res;
  pop(&res);
  printf("result: %s\n", res);
  destroy_connection();
  return 0;
}
