#include <stdio.h>
#include <stdlib.h>

#include "mq_client.h"

void usage(){
  printf("Usage:\n");
  printf(" -h<hostname>\n");
  printf(" -p<port>\n");
  printf(" -o [Pop]\n");
  printf(" -u [Push]<body>\n");
  exit (8);
}


int main(int argc, char * argv[]){
  int count;
  int bpop = 0;
  char *body = NULL;
  char *host = NULL;
  char *port = NULL;
  if (argc < 2){
    usage();
  }

  for(count = 1; count < argc; count ){
    printf("argv: %s\n", argv[count]);
    if (argv[count][0] != '-')
      usage();
    switch(argv[count][1]){
    case 'h':
      host = argv[++count];
      break;
    case 'p':
      port = argv[++count];
      break;
    case 'u':
      body = argv[++count];
      break;
    case 'o':
      bpop = 1;
      break;
    }
    count++;

  }

  struct mq_client * mq = initialize_mq_client(host, port);

  if(bpop){
    char * buf;
    pop(mq, &buf);
    printf("POPPED: %s\n", buf);
    free(buf);
  }

  return 0;
}
