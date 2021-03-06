#include <stdio.h>
#include <stdlib.h>

#include "mq_client.h"
#include "client.h"

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
  int bpush = 0;
  char *body = NULL;
  char *host = NULL;
  char *port = NULL;

  struct client * c;

  if (argc < 2){
    //usage();
  }

  for(count = 1; count < argc; ){
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
      bpush = 1;
      body = argv[++count];
      break;
    case 'o':
      break;
    }
    count++;

  }
  if (host == NULL)
    host = "localhost";

  if (port == NULL)
    port = "3443";

  c = initialize_client(host, port);

  if(bpush){
    if (body == NULL)
      usage();
    push(c, body);
  } else {
    char * buf;
    pop(c, &buf);
    printf("%s\n", buf);
    free(buf);
  }

  free(c);
  return 0;
}
