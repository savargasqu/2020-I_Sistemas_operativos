#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

  pid_t pid;
  int r;
  int pipefd[2];
  char buf[11];

  r = pipe(pipefd);
  if (r < 0) {
    perror("Error pipe: ");
    exit(-1);
  }
  pid = fork();
  if (pid < 0) {
    perror("Error fork: ");
    exit(-1);
  }

  if (pid == 0) { // Child process writes to pipe
    close(pipefd[0]);
    write(pipefd[1], "Hola mundo", 10);
    close(pipefd[1]);
    perror("Error fork: ");
    exit(0);
  } else { // Parent process reads from file
    close(pipefd[1]);
    r = read(pipefd[0], buf, 10);
    buf[r] = 0;
    //handle errors
    close(pipefd[0]);
    printf("\nMensaje de proceso hijo: %s\n", buf);
    exit(0);
  }
  return 0;
}
