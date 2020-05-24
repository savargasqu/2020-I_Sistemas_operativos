#include "times_of_IPC.h"

int main(int argc, char *argv[]) {
  int r, pipefd[2]; // Pipe file descriptor
  int rand_num;
  pid_t pid;

  int num_ints = atoi(argv[1]); // Number of integers (4 bytes) to transmit.

  r = pipe(pipefd); // Create pipe
  if (pipefd < 0) {
    perror("Error in pipe() ");
    exit(-1);
  }

  pid = fork(); // Create child process
  if (pid == -1) {
    perror("Error in fork() ");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {     // child writes to pipefd[1]
    close(pipefd[0]); // Prevent the other end of being used

    srand((unsigned)time(NULL)); // set the seed
    for (int i = 0; i < num_ints; ++i) {
      rand_num = rand();
      r = write(pipefd[1], &rand_num, sizeof(rand_num));
      if (r <= 0) {
        perror("Error writing to pipe");
        exit(EXIT_FAILURE);
      }
      // printf("WRITE: %d: %d\n", i, rand_num); // For debugging
    }

    close(pipefd[1]);
    exit(0);

  } else { // parent reads from pipefd[0]
    close(pipefd[1]);

    int i;
    for (i = 0; i < num_ints; ++i) {
      r = read(pipefd[0], &rand_num, sizeof(rand_num));
      if (r <= 0) {
        perror("Error reading pipe");
        exit(EXIT_FAILURE);
      }
      // printf("READ: %d: %d\n", i, rand_num); // For debugging
    }
    printf("%d numbers were read.\n", i);

    close(pipefd[0]);
  }

  return 0;
}
