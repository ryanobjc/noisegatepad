#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void checkerr(int r, const char * msg) {
  if (r < 0) {
    fprintf(stderr, "%s: (%d) %s\n", msg, errno, strerror(errno));
    exit(1);
  }
}


int main(int argc, char ** argv) {
  // TOOD use getprotoent?
  int s = socket(PF_INET, SOCK_DGRAM, 0);

  checkerr(s, "Opening socket");

  struct sockaddr_in gateaddr;
  bzero((char *)&gateaddr, sizeof(gateaddr));
  gateaddr.sin_family = AF_INET;
  // COMMAND LINE CFG
  gateaddr.sin_port = htons(30012);
  // COMMAND LINE CFG
  gateaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int r = connect(s, (struct sockaddr *)&gateaddr, sizeof(gateaddr));
  struct timeval tv;
  tv.tv_sec = 3;

  //setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval * )&tv, sizeof(tv));

  checkerr(r, "Connecting");

  const char * msg = "OPEN!";
  r = send(s, msg, strlen(msg), 0);

  checkerr(r, "Writing");

  char buf[2048];
  // can get EAGAIN, connection refused here.
  ssize_t r2 = recv(s, buf, 2048, 0);
  checkerr(r2, "Reading");
  
  printf("Read: %d\n", r2);
}
