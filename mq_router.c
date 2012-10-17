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

  if (!strcmp(command, "PUSH")) {
    printf("PUSHED!!");
  } else if (!strcmp(command, "POP")) {
    printf("POPPED");
  } else {
    printf("don't know what we did");
  }
}
