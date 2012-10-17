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
    printf("PUSHED!!\n");
    add_to_queue(queue, body);
  } else if (!strcmp(command, "POP")) {
    printf("POPPED\n");
    char * tmp;
    remove_from_queue(queue, result);
    printf("TMP: %s\n", tmp);
  } else {
    printf("don't know what we did\n");
  }
}
