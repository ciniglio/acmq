#include "mq_router.h"
#include "queue.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Queue * initialize_queue(){
  struct Queue * queue = malloc(sizeof(struct Queue));
  init_queue(queue);
  return queue;
}

void handle_string(char * str, char ** result, void * state){
  struct Queue * queue = (struct Queue *) state;
  *result = NULL;
  if (queue == NULL)
    initialize_queue();

  char *command, *body;
  command = strtok_r(str, " ", &body);

  if (command != NULL &&
      !strncmp(command, PUSH, PUSHLEN)){
    add_to_queue(queue, body);
  } else if (str != NULL &&
             !strncmp(str, POP, POPLEN)){
    remove_from_queue(queue, result);
  } else {
    printf("don't know what we did\n");
  }
}
