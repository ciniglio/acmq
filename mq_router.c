#include "mq_router.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>

void handle_string(char * str){
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
