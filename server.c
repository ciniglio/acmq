#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void set_hints_for_streaming(struct addrinfo * hints){
  memset(hints, 0, sizeof *hints); //clear hints struct
  hints->ai_family = AF_UNSPEC; //either IP4 or 6
  hints->ai_socktype = SOCK_STREAM; // TCP
  hints->ai_flags = AI_PASSIVE; // use my IP
}

int bind_to_servinfo(struct addrinfo * servinfo, struct addrinfo * p){
  int yes=1;
  int socket_fd;

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((socket_fd = socket(p->ai_family, p->ai_socktype,
                            p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                   sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
      close(socket_fd);
      perror("server: bind");
      continue;
    }

    break;
  }

  return socket_fd;
}

int create_server(void (*callback)(char *, char **)) {

  int socket_fd, new_connection_fd;
    // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];
  int rv;

  set_hints_for_streaming(&hints);

  // get my addrinfo in servinfo
  if ((rv = getaddrinfo(NULL, "3443", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  socket_fd = bind_to_servinfo(servinfo, p);

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (listen(socket_fd, 10) == -1) {
    perror("listen");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_connection_fd = accept(socket_fd,
                               (struct sockaddr *)&their_addr,
                               &sin_size);

    if (new_connection_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *)&their_addr),
              s,
              sizeof s);
    printf("server: got connection from %s\n", s);

    char * buf = malloc(sizeof(char)*256);
    char * result;
    recv(new_connection_fd, buf, 256, 0);
    free(buf);
    callback(buf, &result);

    if (send(new_connection_fd, "Hello, world!\n", 13, 0) == -1)
      perror("send");
    close(new_connection_fd);
  }
  return 0;
}
