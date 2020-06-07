#include "../p2.h"

void handle_error(char *fn_name) {
  perror(sprintf("\n--> Error in %s", fn_name));
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
}

void recv_record(int socketfd, dogType *p_dog) {
  if (recv(socketfd, p_dog, sizeof(dogType), 0) < 0)
    handle_error("recv");
}

void send_id(int clientfd, unsigned id) {
  if (send(clientfd, &id, sizeof(unsigned), 0) < 0)
    handle_error("send");
}

unsigned recv_id(int socketfd) {
  unsigned id;
  if (recv(socketfd, &id, sizeof(unsigned), 0) < 0)
    handle_error("recv");
  return id;
}

void send_name(int socketfd) {
  char *name = ask_name();
  if (send(socketfd, name, sizeof(char) * 32, 0) < 0)
    handle_error("send");
  free(name);
}

char *recv_name(int socketfd) {
  char *name = malloc(sizeof(char) * 32);
  if (recv(socketfd, name, sizeof(char) * 32, 0) < 0)
    handle_error("recv");
  return name;
}


/* relay_file_contents: Write the contents from inputfd to outputfd */
void relay_file_contents(int inputfd, int outputfd) {
  char *buffer = (char *)malloc(sizeof(char) * 512);
  // char buffer[512] = ""; // todo: Make size define
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
}
