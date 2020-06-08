#include "../p2.h"
#define BACKLOG 2
#define NUM_THREADS 32

int main() {
  /* Initialize table */
  table_t *table = open_table_file(false);
  generate_random_table(table); // For debugging
  /* Declare thread "pool" */
  pthread_t tid[NUM_THREADS];
  thr_info_t thread_info; // Struct passed to thread
  /* Declare socket file descriptors and helper structs */
  int serverfd, clientfd;           // Listen on serverfd for clientfd
  struct addrinfo hints, *servinfo; // To create socket automatically
  struct sockaddr_storage cli_addr; // connector's address information
  socklen_t addr_size;
  int yes = 1;              // For setsockopt()
  char s[INET6_ADDRSTRLEN]; // For debugging
  /* Write hints for the getaddrinfo function */
  memset(&hints, 0, sizeof(hints)); // Fill the struct with 0s before beginning
  hints.ai_family = AF_UNSPEC;      // Let the socket use either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // Stream socket (TCP)
  hints.ai_flags = AI_PASSIVE;      // Fill in the IP automatically
  if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
    handle_error("server.getaddrinfo");
  /* Set up server socket */
  // Reuse port.
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  // Create a new socket for the server
  serverfd =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if (serverfd < 0)
    handle_error("socket");
  // Associate the socket with a port.
  if (bind(serverfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    handle_error("bind");
  // Listen for incoming connections, queue them.
  if (listen(serverfd, BACKLOG) < 0)
    handle_error("listen");
  freeaddrinfo(servinfo); // all done with this structure

  printf("server: waiting for connections...\n"); // For debugging

  /* Main accept loop */
  int i = 0;
  while (true) {
    addr_size = sizeof(clientfd); // Set size to client
    // Get pending connection, returns new socket descriptor.
    clientfd = accept(serverfd, (struct sockaddr *)&cli_addr, &addr_size);
    if (clientfd < 0) {
      perror("accept");
      continue; // DON'T crash if a connection fails
    }
    // For debugging
    inet_ntop(cli_addr.ss_family, get_in_addr((struct sockaddr *)&cli_addr), s,
              sizeof s);
    printf("server: %s has connected.\n", s);
    // so the main thread can entertain next request

    thread_info.socketfd = clientfd;
    thread_info.table = table;
    thread_info.IP = s;
    if (pthread_create(&tid[i], NULL, srv_menu, &thread_info) != 0)
      handle_error("thread");
    // srv_menu(table, clientfd, s);
    //        if( i >= 50)

    if (i >= NUM_THREADS) {
      i = 0;
      while (i < NUM_THREADS) {
        pthread_join(tid[i++], NULL);
      }
      i = 0;
    }

    printf("server: %s has disconnected.\n", s); // For debugging
  }                                              // end while

  close(clientfd);
  close_table_file(table);
  return 0;
}

/**** MODULE: SERVER SIDE NETWORK FUNCTIONS ****/

// void srv_menu(table_t *ht, int clientfd, char *IP) {
void *srv_menu(void *args) {
  thr_info_t *t_info = (thr_info_t *)args;
  table_t *ht = t_info->table;
  int clientfd = t_info->socketfd;
  char *IP = t_info->IP;

  dogType *temp = allocate_record();
  FILE *log = open_log();
  unsigned id;
  char *name, *op;
  int menu_selection = -1;
  while (true) {
    if (recv(clientfd, &menu_selection, sizeof(int), 0) < 0)
      handle_error("recv");
    printf("user input: %d\n", menu_selection); // For debugging
    switch (menu_selection) {
    case 1:
      op = "insert";
      recv_record(clientfd, temp);
      id = probe_table(ht, poly_hash(temp->name));
      srv_insert(clientfd, ht, temp, id);
      break;
    case 2:
      op = "view";
      send_id(clientfd, ht->size); // Send table size
      id = recv_id(clientfd);
      srv_view(clientfd, ht, id);
      break;
    case 3:
      op = "delete";
      send_id(clientfd, ht->size); // send table size
      id = recv_id(clientfd);
      srv_delete(clientfd, ht, id);
      break;
    case 4: // Buscar registro
      op = "search";
      name = recv_name(clientfd);
      srv_search(clientfd, ht, name);
      break;
    case 5: // Salir
      close_log(log);
      free(temp);
      return NULL;
    } // end switch
    write_to_log(log, op, name, id, IP);
  } // end while
  close_log(log);
  free(temp);
  return NULL;
}

void srv_insert(int socketfd, table_t *ht, dogType *temp, unsigned id) {
  if (insert_record(ht, temp, id)) {
    send_id(socketfd, id);
  } else {
    send_id(socketfd, NUM_RECORDS + 1);
  }
}

void srv_view(int socketfd, table_t *ht, unsigned id) {
  dogType *temp = allocate_record();
  bool ok, found = false;
  // Check if record exists
  found = view_record(ht, temp, id);
  if (send(socketfd, &found, sizeof(bool), 0) < 0)
    handle_error("server.send");
  if (!found) {
    return;
  }
  send_record(socketfd, temp);
  // Wait for confirmation to send medical history
  recv(socketfd, &ok, sizeof(bool), 0);
  if (ok) {
    send_medical_hist(socketfd, id);
  }
  free(temp);
}

void srv_delete(int socketfd, table_t *ht, unsigned id) {
  dogType *temp = allocate_record();
  delete_record(ht, temp, id);
  free(temp);
}

void srv_search(int socketfd, table_t *table, char *name) {
  dogType *temp = allocate_record();
  unsigned pos = poly_hash(name);
  unsigned limit = NUM_RECORDS;
  lookup_in_table(table, pos);
  for (unsigned i = pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, name) == 0) {
      // If a match is found, send it through the socket
      send_id(socketfd, i);
      send_record(socketfd, temp);
    } else if (strcmp(temp->name, "") == 0) {
      // uninitialized record was found, stop the search
      send_id(socketfd, NUM_RECORDS + 1);
      free(temp);
      return;
    }
    if (i == NUM_RECORDS - 1) {
      i = 0;
      lookup_in_table(table, 0);
      limit = pos;
    }
  }
  free(temp);
}

/* send_medical_hist: Send the file contents corresponding to the ID
 * aux to srv_view */
void send_medical_hist(int socketfd, unsigned id) {
  bool confirm;
  // Retrive text file
  FILE *fp;
  char *file_name = (char *)malloc(sizeof(char) * 24);
  sprintf(file_name, "data/%u.txt", id);
  // Try to open file
  if ((fp = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");
  // Send data to socket: file => buffer => socket
  send_file(socketfd, file_name);
  // relay_file_contents(fileno(fp), socketfd);
  // Wait for confirmation
  if (recv(socketfd, &confirm, sizeof(bool), 0) < 0)
    handle_error("server.recv");
  if (confirm) {
    // Write updated data back to file: socket => buffer => file
    recv_file(socketfd, file_name);
    // relay_file_contents(socketfd, fileno(fp));
  }
  // Close file
  if (fclose(fp) != 0)
    handle_error("fclose");
}
