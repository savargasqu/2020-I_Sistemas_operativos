#include "times_of_IPC.h"

int main(int argc, char *argv[]) {
  pid_t pid;
  int r, status, shmId, *ap;

  int num_ints = atoi(argv[1]); // Number of integers (4 bytes) to transmit.

  // Get shared memory space ID (only child processes can use IPC_PRIVATE)
  shmId = shmget(IPC_PRIVATE, num_ints * sizeof(int), 0666 | IPC_CREAT);
  if (shmId < 0) {
    perror("Error in shmget() ");
    exit(EXIT_FAILURE);
  }
  // Map shared memory to this process
  ap = (int *)shmat(shmId, 0, 0); 

  pid = fork();
  if (pid == -1) {
    perror("Error in fork() ");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // Child process produces the data
    srand((unsigned)time(NULL));
    for (int i = 0; i < num_ints; i++) {
      ap[i] = rand();
    }

  } else {                      // Parent process consumes the data
    if (wait(&status) == pid) { // wait for child termination
      int i;
      for (i = 0; i < num_ints; i++) {
        if (&ap[i] == NULL) { // Check all values where actually shared
          break;
        }
      }
      printf("%d numbers were read.\n", i);
    }
  }

  // Unmap shared memory
  r = shmdt(ap); 
  if (r < 0) {
    perror("Error in shmdt() ");
    exit(EXIT_FAILURE);
  }

  return 0;
}
