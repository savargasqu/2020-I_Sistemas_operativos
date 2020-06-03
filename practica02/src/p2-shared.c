#include "../include/p2-shared.h"

void handle_error(char* func_name) {
  perror(sprintf("\n--> Error in %s", func_name));
  exit(-1);
}

/* allocate_record: Wrapper around malloc(dogType) */
dogType *allocate_record() {
  dogType *new_record = (dogType *)malloc(sizeof(dogType));
  if (new_record == NULL)
    handle_error("malloc");

  return new_record;
}

void send_record(int clientfd, dogType *p_dog) {
  if (send(clientfd, p_dog, sizeof(dogType), 0) < 0)
    handle_error("send");
}

void recv_record(int socketfd, dogType *temp) {
  if (recv(socketfd, temp, sizeof(dogType), 0) < 0)
    handle_error("recv");
}

/* relay_file_contents: Write the contents from inputfd to outputfd */
void relay_file_contents(int inputfd, int outputfd, char *buffer) {
  while (1) {
    // Read data into buffer. We may not have enough to fill up buffer, so we
    // store how many bytes were actually read in bytes_read.
    int bytes_read = read(inputfd, buffer, sizeof(buffer));
    if (bytes_read == 0) { // We're done reading from the file
      break;
    } else if (bytes_read < 0) {
      handle_error("read");
    }

    // You need a loop for the write, because not all of the data may be
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

