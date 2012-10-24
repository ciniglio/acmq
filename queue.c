#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "mq_persist.h"

int add_to_queue(struct Queue * q, char* body){
  int ret = _add_to_queue(q, body);
  save_mq(q);
  return ret;
}

int _add_to_queue(struct Queue * q, char* body){
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
    n->prev = NULL;
    n->next = NULL;
  } else {
    n->prev = q->last;
    n->next = NULL;
    q->last->next = n;
    q->last = n;
    q->len++;
  }

  return 0;
}

int remove_from_queue(struct Queue * q, char **result){
  int ret = _remove_from_queue(q, result);
  save_mq(q);
  return ret;
}

int _remove_from_queue(struct Queue * q, char **result){
  if (q->len == 0){
    *result = NULL;
    return 0;
  }

  struct Node * n = q->first;
  int size = n->body_size;
  *result = malloc((n->body_size + 1) * sizeof(char));
  if (*result == NULL)
    return -1;
  strlcpy(*result, n->body, n->body_size+1);

  q->first = q->first->next;
  if (q->first != NULL)
    q->first->prev = NULL;
  q->len--;

  free(n->body);
  free(n);

  return size;
}

struct Queue * _initialize_queue(){
  struct Queue * queue = malloc(sizeof(struct Queue));
  queue->len = 0;
  return queue;
}

struct Queue * initialize_queue(){
  struct Queue * q = _initialize_queue();
  if(open_mq(q) != 0){
    perror("Open queue");
    exit(10);
  }
  return q;
}

struct Queue * copy_queue(struct Queue * q){
  struct Queue * p = _initialize_queue();
  struct Node * n;
  for(n = q->first; (n && q->len != 0);){
    _add_to_queue(p, n->body);
    n = n->next;
  }
  return p;
}

int main_test(){
  // test functions
  struct Queue q = *(initialize_queue());

  add_to_queue(&q, "Alejandro");
  printf("Queue size: %d\n", q.len);
  add_to_queue(&q, "Darren");
  printf("Queue size: %d\n", q.len);

  char * result;
  remove_from_queue(&q, &result);
  printf("Queue size: %d\n", q.len);
  printf("Queue result: %s\n", result);
  free(result);

  char * result2;
  remove_from_queue(&q, &result2);
  printf("Queue size: %d\n", q.len);
  printf("Queue result: %s\n", result2);
  free(result2);
  return 0;
}
