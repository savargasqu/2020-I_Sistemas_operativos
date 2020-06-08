#include "../p2.h"

void handle_error(char *fn_name) {
  char *err_str;
  sprintf(err_str, "\n--> Error en %s", fn_name);
  perror(err_str);
  exit(-1);
}

/* allocate_record: Wrapper around malloc(dogType) */
dogType *allocate_record() {
  dogType *new_record = (dogType *)malloc(sizeof(dogType));
  if (new_record == NULL)
    handle_error("malloc");

  return new_record;
}

void send_record(int socketfd, dogType *p_dog) {
  if (send(socketfd, p_dog, sizeof(dogType), 0) < 0)
    handle_error("send");
  printf("sent %s\n", p_dog->name);
}

void recv_record(int socketfd, dogType *p_dog) {
  if (recv(socketfd, p_dog, sizeof(dogType), 0) < 0)
    handle_error("recv");
  printf("received %s\n", p_dog->name);
}

void send_id(int clientfd, unsigned id) {
  if (send(clientfd, &id, sizeof(unsigned), 0) < 0)
    handle_error("send");
  printf("sent %u\n", id);
}

unsigned recv_id(int socketfd) {
  unsigned id;
  if (recv(socketfd, &id, sizeof(unsigned), 0) < 0)
    handle_error("recv");
  printf("received %u\n", id);
  return id;
}

void send_name(int socketfd, char *name) {
  if (send(socketfd, name, sizeof(char) * 32, 0) < 0)
    handle_error("send");
  printf("sent %s\n", name);
  free(name);
}

char *recv_name(int socketfd) {
  char *name = malloc(sizeof(char) * 32);
  if (recv(socketfd, name, sizeof(char) * 32, 0) < 0)
    handle_error("recv");
  printf("received %s\n", name);
  return name;
}

void send_file(int socketfd, char *file_name) {
  char buff[MAX];
  FILE *fp;
  fp = fopen("sent.txt", "r+");
  if (fp == NULL) { // If file does not exist, create it.
    fp = fopen("sent.txt", "w");
  }
  // Write to socket
  if (fgets(buff, MAX, fp) != NULL) { // Fgets reads upto MAX character or EOF.
    write(socketfd, buff, sizeof(buff)); // Send the file data to stream.
  }
  if (fclose(fp) != 0)
    handle_error("fclose");
  printf("File sent successfully.\n"); // For debugging
}

void recv_file(int socketfd, char *file_name) {
  char buff[MAX]; // to store message from client
  FILE *fp;
  if ((fp = fopen(file_name, "w+")) <= 0)
    handle_error("fopen");
  // Read from socket
  if (read(socketfd, buff, MAX) > 0) {
    fprintf(fp, "%s", buff);
  }
  if (fclose(fp) != 0)
    handle_error("fclose");
  printf("File received successfully.\n"); // For debugging
}

/* relay_file_contents: Write the contents from inputfd to outputfd */
void relay_file_contents(int inputfd, int outputfd) {
  char *buffer = (char *)malloc(sizeof(char) * 128);
  while (true) {
    // Read data into buffer. We may not have enough to fill up buffer, so we
    // store how many bytes were actually read in bytes_read.
    int bytes_read = read(inputfd, buffer, sizeof(buffer));
    if (bytes_read == 0) { // We're done reading from the file
      break;
    } else if (bytes_read < 0) {
      handle_error("read");
    }

    // Loop for the write, because not all of the data may be
    // written in one call; write will return how many bytes were written. p
    // keeps track of where in the buffer we are, while we decrement
    // bytes_read to keep track of how many bytes are left to write.
    void *p = buffer;
    while (bytes_read > 0) {
      int bytes_written = write(outputfd, p, bytes_read);
      if (bytes_written <= 0) {
        handle_error("write");
      }
      bytes_read -= bytes_written;
      p += bytes_written;
    }
  }
  free(buffer);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
