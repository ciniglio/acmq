#include "mq_router.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Queue * queue;

void initialize_queue(){
  queue = malloc(sizeof(struct Queue));
  init_queue(queue);
}

void handle_string(char * str, char ** result){
  if (queue == NULL)
    initialize_queue();

  char *command, *body;
  command = strtok_r(str, " ", &body);

  if (command != NULL &&
      !strncmp(command, "PUSH", 4)) {
    add_to_queue(queue, body);
  } else if (str != NULL &&
             !strncmp(str, "POP", 3)) {
    remove_from_queue(queue, result);
  } else {
    printf("don't know what we did\n");
  }
}
