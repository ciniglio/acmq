#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3443" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

int get_tcp_ai(char* host, char* port, struct addrinfo **res);
int get_socket_from_ai(struct addrinfo * servinfo, struct addrinfo ** res);

struct Connection {
  int sockfd;
  struct addrinfo * si;
  struct addrinfo * p;
};

struct Connection *c = NULL;

int init_connection(char* host, char* port){
  if (c == NULL){
    c = malloc(sizeof(struct Connection));
    if (c == NULL){
      perror("allocating Connection");
      return 1;
    }

    if (get_tcp_ai(host, port, &(c->si)) != 0) {
      perror("get TCP Addrinfo");
    }

    c->sockfd = get_socket_from_ai(c->si, &(c->p));

    if (c->p == NULL) {
      fprintf(stderr, "client: failed to connect\n");
      free(c);
      return 1;
    }
  }
  return 0;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
        if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
        }

        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_tcp_ai(char* host, char* port, struct addrinfo **res){
  struct addrinfo hints;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(host, port, &hints, res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  return 0;

}

int get_socket_from_ai(struct addrinfo * servinfo, struct addrinfo ** res){
  // loop through all the results and connect to the first we can
  int sockfd;
  for(*res = servinfo; *res != NULL; *res = (*res)->ai_next) {
    if ((sockfd = socket((*res)->ai_family, (*res)->ai_socktype,
                         (*res)->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(sockfd, (*res)->ai_addr, (*res)->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }
  return sockfd;
}

void print_connection_info(){
  struct addrinfo *p = c->p;
  char s[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);

  printf("client: connecting to %s\n", s);
}

void destroy_connection(){
  freeaddrinfo(c->si);
  close(c->sockfd);

  free(c);
  c = NULL;
}

/* Caller is responsible for making sure that buf is null terminated */
int send_data_through_connection(char * buf){
  int len = strlen(buf);
  if (send(c->sockfd, buf, len, 0) == -1)
    perror("send");
  return 0;
}


/* returns number of bytes recieved. If negative, there was an error */
int recv_data_from_connection(char ** res){
  int numbytes;
  char buf[MAXDATASIZE];

  if ((numbytes = recv(c->sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  buf[numbytes] = '\0';
  *res = malloc(sizeof(char) * (numbytes+1));
  if (*res == NULL){
    perror("allocating buffer");
    return -1;
  }

  strlcpy(*res, buf, numbytes);

  printf("client: received '%s' :: %d\n",buf, numbytes);
  return numbytes;
}

int main(int argc, char *argv[])
{
        char *buf;

        if (argc != 2) {
            fprintf(stderr,"usage: client hostname\n");
            exit(1);
        }
        init_connection(argv[1], PORT);
        print_connection_info();

        send_data_through_connection("PUSH it on");
        recv_data_from_connection(&buf);
        free(buf);

        destroy_connection();
        return 0;
}
