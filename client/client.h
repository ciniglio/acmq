int init_connection(char* host, char* port);
void destroy_connection();

int send_data_through_connection(char * buf);
int recv_data_from_connection(char ** res);
