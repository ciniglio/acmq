#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

struct mq_client {
  char * host;
  char * port;
};

struct mq_client * initialize_mq_client(char * host, char *port){
  struct mq_client *mq = malloc(sizeof(struct mq_client));
  mq->host = malloc(sizeof(char) * (strlen(host) + 1));
  mq->port = malloc(sizeof(char) * (strlen(port) + 1));
  strlcpy(mq->host, host, strlen(host) + 1);
  strlcpy(mq->port, port, strlen(port) + 1);

  return mq;
}

int pop(struct mq_client * mq, char ** res){
  init_connection(mq->host, mq->port);
  if(send_data_through_connection("POP") != 0){
    perror("sending");
    destroy_connection();
    return 1;
  }
  char * long_res;

  if (recv_data_from_connection(&long_res) < 0){
    perror("Recieving");
    destroy_connection();
    return 1;
  }

  int len = strlen(long_res);
  *res = malloc(sizeof(char) * (len + 1));
  strlcpy(*res, long_res, len + 1);
  free(long_res);

  destroy_connection();
  return 0;
}

// buf must be a NULL terminated string
int push(struct mq_client * mq, char * buf){
  init_connection(mq->host, mq->port);
  int len = strnlen(buf, MAX_STRING_LENGTH);
  char * str = malloc(sizeof(char) * (len + 5 + 1));
  if(str == NULL){
    perror("alloc");
    destroy_connection();
    return 1;
  }
  strlcpy(str, "PUSH ", 6);
  strlcat(str, buf, len + 5 + 1);

  if(send_data_through_connection(str) != 0){
    perror("sending");
    destroy_connection();
    return 1;
  }

  free(str);

  char * res;
  if(recv_data_from_connection(&res) != 4 ||
     strncmp(res, "ACK", 3) != 0){
    perror("No ACK");
    destroy_connection();
    return 1;
  }
  destroy_connection();
  return 0;
}

int main(int argc, char *argv[]){
  struct mq_client * mq = initialize_mq_client("localhost", "3443");

  push(mq, "this on");

  char * res;
  pop(mq, &res);
  printf("result: %s\n", res);
  free(res);
  return 0;
}
