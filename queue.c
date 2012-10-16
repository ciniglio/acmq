#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

int add_to_queue(struct Queue * q, char* body){
  // build the node
  struct Node n;
  int body_size = strlen(body);
  n.body_size = body_size;
  n.body = malloc(body_size * sizeof(char));
  if (n.body == 0)
    return 1;
  strncpy(n.body, body, body_size);


  // insert the node in the correct location.
  if (q->len == 0){
    q->first = &n;
    q->last = &n;
  } else {
    n.prev = q->last;
    q->last->next = &n;
    q->last = &n;
  }
  return 0;
}

int main () {
  // test functions
  struct Queue q;
  q.len = 0;
  return 0;
}
