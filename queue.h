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

struct Queue * initialize_queue();
struct Queue * copy_queue(struct Queue *);
int add_to_queue(struct Queue * q, char* body);
int remove_from_queue(struct Queue * q, char **result);

int _add_to_queue(struct Queue * q, char* body);
int _remove_from_queue(struct Queue * q, char **result);
