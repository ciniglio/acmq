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

int main(int argc, char *argv[])
{
        int sockfd, numbytes;
        char buf[MAXDATASIZE];
        struct addrinfo *servinfo, *p;
        char s[INET6_ADDRSTRLEN];

        if (argc != 2) {
            fprintf(stderr,"usage: client hostname\n");
            exit(1);
        }

        if (get_tcp_ai(argv[1], PORT, &servinfo) != 0) {
          perror("get TCP Addrinfo");
        }

        sockfd = get_socket_from_ai(servinfo, &p);

        if (p == NULL) {
          fprintf(stderr, "client: failed to connect\n");
          return 2;
        }

        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                  s, sizeof s);

        printf("client: connecting to %s\n", s);

        freeaddrinfo(servinfo); // all done with this structure

        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("client: received '%s'\n",buf);

        close(sockfd);

        return 0;
}
