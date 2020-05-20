#include <assert.h>
#include <arpa/inet.h>  // Sockets
#include <netdb.h>      // Sockets
#include <netinet/in.h> // Sockets
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Standard string lib
#include <strings.h> // Non-standard BSD-related lib. needed for bzero
#include <sys/shm.h> // Shared memory
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> // Rand
#include <unistd.h>

#define PORT 3535
#define BACKLOG 2
