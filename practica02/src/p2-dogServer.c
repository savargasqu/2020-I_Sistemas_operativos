#include "../p2.h"
#define BACKLOG 2
#define NUM_THREADS 32

int main() {
  // Hash table file
  table_t *table = open_table_file(); // Initialize table
  generate_random_table(table);       // Fill table

  // Thread "pool"
  pthread_t thread[NUM_THREADS]; // Array of threads
  int i = 0;

  struct sockaddr_storage client;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int yes = 1;               // For setsockopt() SO_REUSEADDR, below.
  int serverfd, clientfd, r; // File descriptors
  char s[INET6_ADDRSTRLEN];
  // Use `getaddrinfo` to build the socket
  // (this makes it network protocol agnostic)
  memset(&hints, 0, sizeof(hints)); // Fill the struct with 0s before beginning
  hints.ai_family = AF_UNSPEC;      // Let the socket use either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // Stream socket (TCP)
  hints.ai_flags = AI_PASSIVE;      // Fill in the IP automatically
  getaddrinfo(NULL, PORT, &hints, &res);
  // Create a new socket for the client will use
  serverfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (serverfd < 0)
    handle_error("socket");
  // Associate a socket with a port.
  if (bind(serverfd, res->ai_addr, res->ai_addrlen) < 0)
    handle_error("bind");
  // Listen for incoming connections, queue them.
  if (listen(serverfd, BACKLOG) < 0)
    handle_error("listen");
  // Reuse port.
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  printf("server: waiting for connections...\n");

  while (1) {                   // Main accept() loop.
    addr_size = sizeof(client); // Set size to client

    // Get pending connection, returns new socket descriptor.
    clientfd = accept(serverfd, (struct sockaddr *)&client, &addr_size);
    if (clientfd < 0)
      handle_error("accept");

    inet_ntop(client.ss_family, (struct sockaddr *)&client, s, sizeof(s));
    printf("server: %s has connected.\n", s);

    srv_menu(table, clientfd); // TODO: table

    printf("server: %s has disconnected.\n", s);
    close(clientfd);
  } // end while

  return 0;
}

/**** MODULE: SERVER SIDE NETWORK FUNCTIONS ****/

void srv_menu(table_t *ht, int clientfd) {
  char menu_selection = '0';
  int id, r;
  dogType *temp = allocate_record();
  char *name;
  while (1) {
    if (recv(clientfd, &menu_selection, sizeof(int), 0) < 0)
      handle_error("recv");
    printf("user input: %c\n", menu_selection); // For debugging

    // TODO: Check success
    switch (menu_selection) {
    case 1:
      srv_insert(ht, clientfd);
      break;
    case 2:                   // Ver registro
      srv_view(ht, clientfd); // Wait for text file request
      break;
    case 3: // Borrar registro
      srv_delete(ht, clientfd);
      break;
    case 4: // Buscar registro
      srv_search(ht, clientfd);
      break;
    case 5: // Salir
      return;
    default:
      printf(INPUT_WARNING);
      break;
    } // end switch
  }   // end while
}

bool srv_insert(table_t *ht, int socketfd) {
  unsigned id;
  bool success;
  dogType *temp = allocate_record();
  // Get record from client and put it in the table
  recv_record(socketfd, temp);
  id = probe_table(ht, poly_hash(temp->name));
  success = insert_record(ht, temp, id);
  free(temp);
  return success; // Insert is't possible
}

bool srv_view(table_t *ht, int socketfd) {
  unsigned id;
  bool ok, found;
  dogType *temp = allocate_record();
  send_id(socketfd, ht->size); // Send table size
  id = recv_id(socketfd);
  if (!view_record(ht, temp, id)) {
    found = false;
    send_record(socketfd, NULL);
  } else {
    send_record(socketfd, temp);
  }
  // Wait for confirmation to send medical history
  recv(socketfd, &ok, sizeof(bool), 0);
  if (ok) {
    send_medical_hist(socketfd, id);
  }
  free(temp);
  return found;
}

void srv_delete(table_t *ht, int socketfd) {
  unsigned id;
  dogType *temp = allocate_record();
  send_id(socketfd, ht->size); // send table size
  id = recv_id(socketfd);
  delete_record(ht, temp, id);
  free(temp);
}

void srv_search(table_t *table, int socketfd) {
  dogType *temp = allocate_record();
  char *name = recv_name(socketfd);
  unsigned pos = poly_hash(name);
  unsigned limit = NUM_RECORDS;
  lookup_in_table(table, pos);
  for (unsigned i = pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, name) == 0) {
      // If a match is found, send it through the socket
      send_record(socketfd, temp);
      send_id(socketfd, i);
    } else if ((strcmp(temp->name, "") == 0) && (temp->deleted == false)) {
      // If a field that has never been initialized is found, stop the search
      send_record(socketfd, NULL);
      break;
    }
    if (i == NUM_RECORDS - 1) {
      i = 0;
      limit = pos;
    }
  }
  free(temp);
}

//void srv_confirm_op(int socketfd) {
//  bool ok;
//  if (send(socketfd, &ok, sizeof(bool), 0) < 0)
//    handle_error("server.send");
//}

/**** SERVER SIDE AUXILIARY FUNCTIONS ****/

/* send_medical_hist: Send the file contents corresponding to the ID
 * aux to srv_view */
void send_medical_hist(int socketfd, unsigned id) {
  char *buff = (char *)malloc(sizeof(char) * 512);
  int confirm;
  // Retrive text file
  char file_name[16];
  sprintf(file_name, "%u.txt", id);
  FILE *temp_file;
  // Try to open file
  if ((temp_file = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");
  // Send data to socket: file => buffer => socket
  relay_file_contents(fileno(temp_file), socketfd);
  // Wait for confirmation
  if (recv(socketfd, &confirm, sizeof(int), 0) < 0) {
    handle_error("server.recv");
  } else if (confirm == 1) {
    // Write updated data back to file: socket => buffer => file
    relay_file_contents(socketfd, fileno(temp_file));
  }
  // Close file
  if (fclose(temp_file) != 0)
    handle_error("fclose");
  free(buff);
}

/* poly_hash: Polynomial hash function for strings */
unsigned poly_hash(char *s) {
  static const unsigned x = 31;
  unsigned int hash_value = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
  }
  return hash_value % NUM_RECORDS; // Hash can't exceed the size of the table
}

/* insert_record: inserts new_record to table. Returns true on success */
bool insert_record(table_t *table, dogType *new_record, unsigned pos) {
  if (0 >= pos && pos <= NUM_RECORDS) {
    lookup_in_table(table, pos); // Set table pointer to the right position
    write_to_table(table, new_record); // Write record to table
    table->size += 1;
    return true; // If operation was successful
  }
  return false; // Insert failed
}

/* view_record: */
bool view_record(table_t *table, dogType *temp, unsigned id) {
  lookup_in_table(table, id);
  read_from_table(table, temp);
  if (strcmp(temp->name, "") == 0) {
    return false;
  } else {
    return true;
  }
}

/* probe_table: Search for an empty slot in the table
 * auxiliary to insert_record */
unsigned probe_table(table_t *table, unsigned pos) {
  unsigned limit = NUM_RECORDS;
  dogType *temp = allocate_record();
  lookup_in_table(table, pos);
  for (unsigned i = pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, "") == 0) {
      // if there's an empty slot, return that position
      free(temp);
      return i;
    }
    if (i == NUM_RECORDS - 1) {
      i = 0;
      limit = pos;
    }
  } // end for
  free(temp);
  return NUM_RECORDS +
         1; // No spot was found/The table is full and needs resizing
}

/* delete_record: */
void delete_record(table_t *table, dogType *temp, unsigned id) {
  lookup_in_table(table, id);
  // Writing a null struct is equivalent to deleting it before a resize
  strcpy(temp->name, "");
  strcpy(temp->species, "");
  strcpy(temp->breed, "");
  temp->age = 0;
  temp->height = 0;
  temp->weight = 0;
  temp->sex = '\0';
  temp->deleted = true;
  write_to_table(table, temp);
  table->size -= 1;
}

