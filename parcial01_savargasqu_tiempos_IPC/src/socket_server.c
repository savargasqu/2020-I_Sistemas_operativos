#include "times_of_IPC.h"

int main(int argc, char *argv[]) {
  int r, serverfd, clientfd, opt = 1;
  int rand_num;
  struct sockaddr_in server, client;
  socklen_t socket_len;
  int num_ints = atoi(argv[1]); // Number of integers (4 bytes) to transmit.

  serverfd = socket(AF_INET, SOCK_STREAM, 0); // Create socket file descriptor
  if (serverfd < 0) {
    perror("Error in socket() ");
    exit(-1);
  }

  // Define the fields of the server struct
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT); // host to network (big endian) byte order
  server.sin_addr.s_addr = INADDR_ANY; // Listen for any connection
  bzero(server.sin_zero, 8); // Write zeros to a byte string (sin_zero)

  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int));

  r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
  if (r < 0) {
    perror("Error in bind() ");
    exit(EXIT_FAILURE);
  }

  r = listen(serverfd, BACKLOG);
  if (r < 0) {
    perror("Error in listen() ");
    exit(EXIT_FAILURE);
  }

  // New file descriptor for the client
  clientfd = accept(serverfd, (struct sockaddr *)&client, &socket_len);
  if (clientfd < 0) {
    perror("Error in accept() ");
    exit(EXIT_FAILURE);
  }

  srand((unsigned)time(NULL)); // set the seed
  for (int i = 0; i < num_ints; i++) {
    rand_num = rand();
    r = send(clientfd, &rand_num, sizeof(int), 0);
    if (r < 0) {
      perror("Error in send() ");
      exit(EXIT_FAILURE);
    }
  }
  close(clientfd);
  close(serverfd);

  return 0;
}
