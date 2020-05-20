#include "times_of_IPC.h"

int main(int argc, char *argv[]) {
  pid_t pid;      // process ID
  FILE *bin_file; // file
  int r, status, rand_num;

  int num_ints = atoi(argv[1]); // Number of integers (4 bytes) to transmit.

  pid = fork();    // Create a child process
  if (pid == -1) { // Error handling
    perror("Error in fork() ");
    exit(EXIT_FAILURE);
  }

  bin_file = fopen("shared_file.dat", "w+");
  if (bin_file <= 0) { // Error handling
    perror("Error in fopen() ");
  }

  if (pid == 0) { // Child/producer process
    srand((unsigned)time(NULL));
    for (int i = 0; i < num_ints; i++) {
      rand_num = rand();
      //printf("%d\n", rand_num); // For debugging
      r = fwrite(&rand_num, sizeof(int), 1, bin_file);
      if (r <= 0) {
        perror("Error in fwrite() ");
        exit(EXIT_FAILURE);
      }
    }
  } else { // Parent/consumer process
    if (wait(&status) == pid) {
      int i;
      for (i = 0; i < num_ints; i++) {
        r = fread(&rand_num, sizeof(int), 1, bin_file);
        if (r <= 0) {
          perror("Error in fread() ");
          exit(EXIT_FAILURE);
        }
        //printf("%d\n", rand_num); // For debugging
      }
      printf("%d numbers were read.\n", i);

    }
  }

  r = fclose(bin_file);
  if (r != 0) {
    perror("Error in fclose() ");
  }

  return 0;
}
