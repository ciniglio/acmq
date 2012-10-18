struct mq_client;
struct mq_client * initialize_mq_client(char * host, char *port);

int pop(struct mq_client * mq, char ** res);
int push(struct mq_client * mq, char * buf);
