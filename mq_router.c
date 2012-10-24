#include "mq_router.h"
#include "queue.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_string(char * str, char ** result, void * state){
  struct Queue * queue = (struct Queue *) state;
  *result = NULL;
  if (queue == NULL)
    exit(1);

  char *cmd, *body, *command_str, *command_body;
  for(command_str = strtok_r(str, "\n", &command_body);
      command_str && (*command_str != EOF);
      command_str = strtok_r(NULL, "\n", &command_body)){

    cmd = strtok_r(command_str, " ", &body);
    if (cmd != NULL &&
        !strncmp(cmd, PUSH, PUSHLEN)){
      add_to_queue(queue, body);
    } else if (str != NULL &&
               !strncmp(command_str, POP, POPLEN)){
      remove_from_queue(queue, result);
    } else {
      printf("BAD INPUT RECIEVED\n");
    }
  }
}
