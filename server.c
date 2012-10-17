#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "server.h"

void sigchld_handler(int s)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {

  /* int listenfd = 0; */
  /* int connfd = 0; */

  /* int status; */
  /* struct addrinfo hints; */
  /* struct addrinfo * servinfo; */

  /* memset(&hints, 0, sizeof(hints)); */
  /* hints.ai_family = AF_UNSPEC; //don't care if ip4 or 6' */
  /* hints.ai_socktype = SOCK_STREAM; //tcp not udp */
  /* hints.ai_flags = AI_PASSIVE; // fill in ip automatically */

  /* if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) { */
  /*   fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status)); */
  /*   exit(1); */
  /* } // servinfo now points to ll of addrinfos */



  /* freeaddrinfo(servinfo); */


  int socket_fd, new_connection_fd;
    // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints); //clear hints struct
  hints.ai_family = AF_UNSPEC; //either IP4 or 6
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE; // use my IP

  // get my addrinfo in servinfo
  if ((rv = getaddrinfo(NULL, "3443", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

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

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (listen(socket_fd, 10) == -1) {
    perror("listen");
    exit(1);
  }

  // set handler function
  sa.sa_handler = sigchld_handler; // reap all dead processes

  // empty out bitmask (used when delivering the signal to handler)
  sigemptyset(&sa.sa_mask);

  // restart process if in the middle of an important call
  sa.sa_flags = SA_RESTART;

  // bind the handler to the SIGCHLD signal
  // (change in state of child proc)
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
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
    printf("sizof %d\n", sizeof(*buf));
    recv(new_connection_fd, buf, 256, 0);

    printf("server: got %s\n", buf);

    // multithreading
    if (!fork()) { // this is the child process
      close(socket_fd); // child doesn't need the listener
      if (send(new_connection_fd, "Hello, world!\n", 13, 0) == -1)
        perror("send");
      close(new_connection_fd);
      exit(0);
    }
    close(new_connection_fd);  // parent doesn't need this
  }

  return 0;

}
