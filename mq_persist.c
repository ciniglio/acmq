#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "persistence.h"
#include "queue.h"

#define FILENAME "mq.save"

int save_mq(struct Queue * q){
  char * str = malloc(sizeof(char));
  struct Queue * p = copy_queue(q);
  strlcat(str, "\0", 1);
  int size = 1;
  char * res;
  size += _remove_from_queue(p, &res);
  while(res != NULL){
    size += 1;
    str = realloc(str, size * sizeof(char));
    strlcat(str, res, size - 1);
    strlcat(str, "\n", size);
    free(res);
    size += _remove_from_queue(p, &res);
  }
  writefile(FILENAME, str, size);
  return 0;
}

int open_mq(struct Queue * q){
  char * str, * tok, * saved;
  readfile(FILENAME, &str);
  if (str == NULL)
    return 0;
  for(tok = strtok_r(str, "\n", &saved);
      tok && (*tok != EOF);
      tok = strtok_r(NULL, "\n", &saved)){
    _add_to_queue(q, tok);
  }
  return 0;
}

int non_main(){
  struct Queue * q = initialize_queue();
  struct Queue * p = initialize_queue();

  add_to_queue(q, "Alejandro");
  add_to_queue(q, "Darren");
  //save_mq(q);
  open_mq(p);
  return 0;
}
