#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "server.h"

#define MAXDATASIZE 1024 // max number of bytes we can get at once
#define MAX_LINE 16384


struct fd_state {
  char * buffer;
  size_t buffer_used;

  int writing;
  size_t n_written;
  size_t write_upto;
};

struct server {
  void (*callback) (char *, char **, void *);
  void * state;
};

struct fd_state * alloc_fd_state(void){
  struct fd_state *state = malloc(sizeof(struct fd_state));
  if (!state)
    return NULL;
  state->buffer_used = state->n_written = state->writing =
    state->write_upto = 0;
  return state;
}

void free_fd_state(struct fd_state *state){
  if (state == NULL)
    return;
  if (state->buffer_used){
    free(state->buffer);
  }
  free(state);
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// get sockaddr, IPv4 or IPv6:
in_port_t get_in_port(struct sockaddr *sa){
  if (sa->sa_family == AF_INET){
    return (((struct sockaddr_in*)sa)->sin_port);
  }

  return (((struct sockaddr_in6*)sa)->sin6_port);
}

void set_hints_for_streaming(struct addrinfo * hints){
  memset(hints, 0, sizeof *hints); //clear hints struct
  hints->ai_family = AF_INET6; //AF_UNSPEC; //either IP4 or 6
  hints->ai_socktype = SOCK_STREAM; // TCP
  hints->ai_flags = AI_PASSIVE; // use my IP
}

int bind_to_servinfo(struct addrinfo * servinfo, struct addrinfo ** p){
  int yes=1;
  int socket_fd;

  // loop through all the results and bind to the first we can
  for(*p = servinfo; *p != NULL; *p = (*p)->ai_next){
    if ((socket_fd = socket((*p)->ai_family, (*p)->ai_socktype,
                            (*p)->ai_protocol)) == -1){
      perror("server: socket");
      continue;
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                   sizeof(int)) == -1){
      perror("setsockopt");
      exit(1);
    }

    if (bind(socket_fd, (*p)->ai_addr, (*p)->ai_addrlen) == -1){
      close(socket_fd);
      perror("server: bind");
      continue;
    }

    break;
  }

  return socket_fd;
}

void make_nonblocking(int fd){
  fcntl(fd, F_SETFL, O_NONBLOCK);
}

void print_connection_info(struct addrinfo *p) {
  char t[INET6_ADDRSTRLEN];
  inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *) p->ai_addr),
            t,
            sizeof t);

  printf("server: waiting for connections on %s:%d...\n",
         t,
         ntohs(get_in_port((struct sockaddr *) p->ai_addr)));

}

int do_read(int sockfd, struct fd_state * state, struct server * s){
  char buf[MAXDATASIZE];
  char * result;
  int num_recieved = recv(sockfd, buf, MAXDATASIZE - 1, 0);
  buf[num_recieved] = '\0';

  s->callback(buf, &result, s->state);
  if (result != NULL){
    state->buffer = result;
    state->buffer_used = 1;
  } else {
    state->buffer_used = 0;
  }
  return 0;
}

int do_write(int sockfd, struct fd_state * state, struct server * s){
  if(state->buffer_used == 1){
    if (send(sockfd, state->buffer, strlen(state->buffer) + 1, 0) == -1){
      perror("send");
      return 1;
    }
  } else {
    if (send(sockfd, "ACK\n", 4, 0) == -1){
      perror("send");
      return 1;
    }
  }
  return 0;
}

int create_server(void (*callback)(char *, char **, void *),
                  char *port,
                  void * state ){

  int socket_fd, new_connection_fd, i;
    // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  int rv;
  fd_set readset, writeset, exset;
  struct fd_state *fdstate[FD_SETSIZE] = {};
  struct server *s = malloc(sizeof(struct server));

  s->callback = callback;
  s->state = state;

  set_hints_for_streaming(&hints);

  // get my addrinfo in servinfo
  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  socket_fd = bind_to_servinfo(servinfo, &p);

  if (p == NULL){
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  if (listen(socket_fd, 10) == -1){
    perror("listen");
    exit(1);
  }

  print_connection_info(p);
  freeaddrinfo(servinfo); // all done with this structure

  while(1){  // main accept() loop
    int maxfd = socket_fd + 1;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    FD_SET(socket_fd, &readset);

    // go through all of our sockets and add them to the readset
    // may want to go back and clear them later instead of doing this
    for (i=0; i < FD_SETSIZE; ++i) {
      if (fdstate[i]) {
        if (i > maxfd)
          maxfd = i;
        FD_SET(i, &readset);
        if (fdstate[i]->writing) {
          FD_SET(i, &writeset);
        }
      }
    }

    if (select(maxfd+1, &readset, &writeset, &exset, NULL) < 0) {
      perror("select");
      return -1;
    }

    // check to see if the listening socket has something new
    // i.e. a new connection
    if (FD_ISSET(socket_fd, &readset)) {
      sin_size = sizeof their_addr;
      new_connection_fd = accept(socket_fd,
                                 (struct sockaddr *)&their_addr,
                                 &sin_size);
      if (new_connection_fd < 0) {
        perror("accept");
      } else if (new_connection_fd > FD_SETSIZE) {
        close(new_connection_fd);
      } else {
        make_nonblocking(new_connection_fd);
        fdstate[new_connection_fd] = alloc_fd_state();
      }
    }

    // go through all the sockets and see if any of them are ready
    for (i=0; i < maxfd+1; ++i) {
      int r = 0;
      if (i == socket_fd)
        continue;

      if (FD_ISSET(i, &readset)) {
        r = do_read(i, fdstate[i], s);
        r = do_write(i, fdstate[i], s);
        free_fd_state(fdstate[i]);
        fdstate[i] = NULL;
        close(i);
      }
      if (r == 0 && FD_ISSET(i, &writeset)) {
        //r = do_write(i, fdstate[i], s);
      }

    }
  }
  return 0;
}
