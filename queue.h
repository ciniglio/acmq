struct Node {
  int body_size;
  struct Node * prev;
  struct Node * next;

  char* body;
};

struct Queue {
  int len;
  struct Node * first;
  struct Node * last;
};

int init_queue(struct Queue * q);
int add_to_queue(struct Queue * q, char* body);
int remove_from_queue(struct Queue * q, char **result);
