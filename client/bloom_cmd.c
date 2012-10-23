#include <stdio.h>
#include <stdlib.h>

#include "bloom_client.h"
#include "client.h"

void usage(){
  printf("Usage:\n");
  printf(" -h<hostname>\n");
  printf(" -p<port>\n");
  printf(" -ba [Bloom_add]<body>\n");
  printf(" -bc [Bloom_check]<body>\n");
  printf(" -bd [Boom_delete]<body>\n");
  exit (8);
}

int main(int argc, char * argv[]){
  int count;
  int bloom_add = 0;
  int bloom_check = 0;
  int bloom_delete = 0;
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
    case 'ba':
      bloom_add = 1;
      body = argv[++count];
    case 'bc':
      bloom_check = 1;
      body = argv[++count];
    case 'bd':
      bloom_delete = 1;
      body = argv[++count];
    }
    count++;

  }
  if (host == NULL)
    host = "localhost";

  if (port == NULL)
    port = "3443";

  c = initialize_client(host, port);

  if(bloom_add){
    if (body == NULL{
    	usage();
    }
    add(c, body);
  } else if(bloom_check){
  	if (body == NULL){
  		usage();
  	}
    check(c, body);
  } else if(bloom_delete){
  	if (body == NULL){
  		usage();
  	}
  	delete(c, body);
  } else {
  	usage();
  }

  free(c);
  return 0;
}
