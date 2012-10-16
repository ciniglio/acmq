#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

int add_to_queue(struct Queue * q, char* body){
  // build the node
  struct Node * n = malloc(sizeof(struct Node));
  int body_size = strlen(body);
  n->body_size = body_size;
  n->body = malloc((body_size + 1) * sizeof(char));
  if (n->body == NULL)
    return 1;
  strlcpy(n->body, body, body_size+1);

  // insert the node in the correct location.
  if (q->len == 0){
    q->first = n;
    q->last = n;
    q->len = 1;
  } else {
    n->prev = q->last;
    q->last->next = n;
    q->last = n;
    q->len++;
  }
  return 0;
}


int init_queue(struct Queue * q){
  q->len = 0;
  return 0;
}

int main () {
  // test functions
  struct Queue q;
  q.len = 0;
  return 0;
}
