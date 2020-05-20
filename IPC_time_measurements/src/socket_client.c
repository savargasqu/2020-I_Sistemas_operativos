#include "times_of_IPC.h"

int main(int argc, char *argv[]) {
  int r, clientfd;
  struct sockaddr_in client;
  struct hostent *he;
  int rand_num;
  int num_ints = atoi(argv[1]); // Number of integers (4 bytes) to transmit.

  clientfd = socket(AF_INET, SOCK_STREAM, 0); // Create socket
  if (clientfd < 0) {
    perror("Error in socket() ");
    exit(EXIT_FAILURE);
  }

  client.sin_family = AF_INET;
  client.sin_port = htons(PORT);

  inet_aton(argv[2], &client.sin_addr); // Read port from command line arguments

  r = connect(clientfd, (struct sockaddr *)&client,
              (socklen_t)sizeof(struct sockaddr));
  if (r < 0) {
    perror("Error in connect() ");
    exit(EXIT_FAILURE);
  }

  int i;
  for (i = 0; i < num_ints; i++) {
    r = recv(clientfd, &rand_num, sizeof(int), 0);
    if (r == -1) {
      perror("Error in recv() ");
    }
  }
  printf("%d numbers were read.\n", i);

  close(clientfd);
  return 0;
}
