#include "../p2-shared.h"

// TODO: This function has to be in a shared module
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

/**** MODULE: CLIENT SIDE ****/
int cli_menu(int clientfd);

void cli_insert(int fd, dogType *p_dog);

void cli_view(int fd, dogType *p_dog, int id);
void display_medical_history(int clientfd, unsigned id);

void cli_delete(int fd, int id);

void cli_search(int fd, dogType *p_dog);

/**** CLIENT SIDE AUXILIARY FUNCTIONS ****/
// TODO: Put besides ask_id
int cli_request_record(int socketfd, dogType *p_dog);
int cli_confirm_operation(int socketfd);

int main() {
  int clientfd, r;
  struct addrinfo hints, *res;

  // Use `getaddrinfo` to build the socket
  // (this makes it network protocol agnostic)
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // Let the socket use either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // Stream socket (TCP)
  hints.ai_flags = AI_PASSIVE;     // Fill in the IP automatically
  getaddrinfo(NULL, PORT, &hints, &res);

  // Create a new socket for the client will use
  clientfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (clientfd < 0)
    handle_error("socket");

  if (connect(clientfd, res->ai_addr, res->ai_addrlen) < 0) {
    handle_error("connect");
  } else {
    printf("Connected to server...\n"); // todo: Print server IP?
  }
  // Go to main menu
  cli_menu(clientfd);

  if (close(clientfd) != 0)
    handle_error("close");
}

int cli_menu(int clientfd) {
  int id, menu_selection = -1;
  char wait;
  char ok[2];
  dogType *temp_dog = allocate_record();

  while (1) {
    system("clear");
    printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
           "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
           "Ingrese el número de una opción: ");

    // Read user input
    scanf("%d", &menu_selection);
    // Tell the server what kind of operation is going to be performed
    send(clientfd, &menu_selection, sizeof(int), 0);

    switch (menu_selection) {
    case 1:                           // Ingresar registro
      ask_new_record(temp_dog);       // Fill dogType info
      cli_insert(clientfd, temp_dog); // Send dogType
      // send_record(clientfd, temp_dog); // Send dogType
      break;

    case 2: // Ver registro
      if ((id = request_record(clientfd, temp_dog)) == -1)
        break;
      cli_view(clientfd, temp_dog, id);
      break;

    case 3: // Borrar registro
      if ((id = request_record(clientfd, temp_dog)) == -1)
        break;
      cli_delete(clientfd, id);
      break;

    case 4: // Buscar registro
      // search_record_by_name(ht);
      break;

    case 5: // Salir
      return 0;
      break;

    default:
      printf("INPUT WARNING");
      break;
    }
    confirm_operation(clientfd);
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait);
  }
}

void cli_insert(int clientfd, dogType *p_dog) {
  // Might refactor this function into a send_record to pair with request_record
  int reply;
  send(clientfd, p_dog, sizeof(dogType), 0); // Send the dog info
}

void cli_view(int clientfd, dogType *temp, int id) {
  char ans;
  print_record(temp, id);
  while (1) {
    printf("Do you want to open the medical history file? [Y/n]\n");
    scanf(" %c", &ans);
    if (ans == 'Y' || ans == 'y') {
      display_medical_history(clientfd, id);
    } else if (ans == 'N' || ans == 'n') {
      break;
    } else {
      printf(INPUT_WARNING);
    } // end if: question
  }   // end while
}

void display_medical_history(int socketfd, unsigned id) {
  char *buff = (char *)malloc(sizeof(char) * 512);

  // Temporary text file the user edits locally before being send to the
  // network
  char file_name[16];
  sprintf(file_name, "%u.txt", id);
  FILE *temp_file;

  // Try to open file
  if ((temp_file = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");

  // Get data from socket: socket => buffer => file
  relay_file_contents(socketfd, fileno(temp_file), buff);

  // Open file in text editor
  system(sprintf("nano %s", id));

  // Write updated data back to socket: file => buffer => socket
  relay_file_contents(fileno(temp_file), socketfd, buff);

  // Close file
  if (fclose(temp_file) != 0)
    handle_error("fclose");
}

void cli_delete(int socketfd, int id) {
  send(socketfd, &id, sizeof(int), 0); // Send the ID of the dog to be deleted
}

void cli_search(int socketfd, dogType *temp) {
  int id;
  char *name = ask_name();
  send(socketfd, name, sizeof(char) * 32, 0); // Send name for search
  while (1) { // Print until there aren't any more structs being send
    if (recv(socketfd, temp, sizeof(dogType), 0) == -1)
      handle_error("client.recv");

    if (recv(socketfd, &id, sizeof(unsigned), 0) == -1)
      handle_error("client.recv");

    if (temp == NULL) {
      break;
    } else {
      print_record(temp, id);
    } // end if
  }   // end while
}

/**** AUXILIARY FUNCTIONS ****/

/* request_record: Requests table size and then a dogType record */
int cli_request_record(int socketfd, dogType *p_dog) {
  unsigned table_size, id;
  // Get table size
  if (recv(socketfd, &table_size, sizeof(int), 0) == -1)
    handle_error("client.recv");

  // Request dog by ID
  id = ask_id(table_size);
  send(socketfd, &id, sizeof(int), 0);

  if (recv(socketfd, p_dog, sizeof(dogType), 0) == -1)
    handle_error("client.recv");

  // Validate dog
  if (strcmp(p_dog->name, "") == 0) {
    printf("No record with ID %u was found\n", id);
    return -1;
  }

  return id;
}

/* ask_id: Prints the size of the table and reads an integer from stdin */
unsigned ask_id(unsigned table_size) {
  unsigned id;
  printf("La tabla tiene %u registros\n", table_size);
  while (1) {
    printf("Ingrese el ID de un registro: ");
    scanf("%u", &id);
    if (id >= 0 && id <= NUM_RECORDS) {
      return id;
    } else {
      printf(INPUT_WARNING);
    }
  }
}

/* ask_new_record: Reads fields from stdin and assigns them to a new_record */
void ask_new_record(dogType *dog_record) {
  fflush(stdin);
  printf("Por favor ingrese los datos solicitados.\n");
  printf("Nombre: ");
  scanf("%s", dog_record->name);
  str_lower_case(dog_record->name);
  printf("Tipo: ");
  scanf("%s", dog_record->species);
  printf("Edad (en años): ");
  scanf("%i", &dog_record->age);
  printf("Raza: ");
  scanf("%s", dog_record->breed);
  printf("Estatura (cm): ");
  scanf("%i", &dog_record->height);
  printf("Peso (Kg): ");
  scanf("%f", &dog_record->weight);
  printf("Sexo (H/M): ");
  scanf(" %c", &dog_record->sex);
}

void str_lower_case(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}
