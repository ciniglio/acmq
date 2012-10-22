struct client;

int pop(struct client * mq, char ** res);
int push(struct client * mq, char * buf);
