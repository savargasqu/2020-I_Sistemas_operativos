#include "../include/p2-dogClient.h"

int main() {
  int clientfd, r;
  struct addrinfo hints, *res;

  // Use `getaddrinfo` to build the socket
  // (this makes it network protocol agnostic)
  memset(&hints, 0, sizeof(hints)); // Fill the struct with 0s before beginning
  hints.ai_family = AF_UNSPEC;      // Let the socket use either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // Stream socket (TCP)
  hints.ai_flags = AI_PASSIVE;      // Fill in the IP automatically

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

/**** MODULE: CLIENT SIDE NETWORK FUNCTIONS ****/

void cli_menu(int clientfd) {
  int id, menu_selection = -1;
  char wait;
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
    case 1:                            // Ingresar registro
      ask_new_record(temp_dog);        // Fill dogType info
      send_record(clientfd, temp_dog); // Send dogType
      break;

    case 2: // Ver registro
      // request_record only returns ID if record exists
      if ((id = request_record(clientfd, temp_dog)) == -1)
        break;
      cli_view(clientfd, temp_dog, id);
      break;

    case 3: // Borrar registro
      if ((id = request_record(clientfd, temp_dog)) == -1)
        break;
      send_id(clientfd, id); // Send ID of record to be deleted
      break;

    case 4: // Buscar registro
      send_name(clientfd); // Ask the user for a name and send it to the server
      cli_search(clientfd, temp_dog); // Print responses
      break;

    case 5: // Salir
      return;
      break;

    default:
      printf(INPUT_WARNING);
      break;
    }
    cli_confirm_operation(clientfd);
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait);
  }
}

void cli_view(int socketfd, dogType *temp, int id) {
  char ans;
  print_record(temp, id);
  while (1) {
    printf("Abrir historial médico? [Y/n]\n");
    scanf(" %c", &ans);
    // Let the server know whether the file will be opened or not
    if (ans == 'Y' || ans == 'y') {
      ans = 'y';
      if (send(socketfd, &ans, sizeof(char), 0) < 0)
        handle_error("client.send");
      display_medical_hist(socketfd, id);
    } else if (ans == 'N' || ans == 'n') {
      if (send(socketfd, &ans, sizeof(char), 0) < 0)
        handle_error("client.send");
      break;
    } else {
      printf(INPUT_WARNING);
    } // end if: question
  }   // end while
}

void cli_search(int socketfd, dogType *temp) {
  int id;
  while (1) { // Print until there aren't any more structs being send
    if (recv(socketfd, temp, sizeof(dogType), 0) < 0)
      handle_error("client.recv");

    if (recv(socketfd, &id, sizeof(unsigned), 0) < 0)
      handle_error("client.recv");

    if (temp == NULL) {
      break;
    } else {
      print_record(temp, id);
    } // end if
  }   // end while
}

void cli_confirm_operation(int socketfd) {
  int res;
  if (recv(socketfd, &res, sizeof(int), 0) < 0)
    handle_error("client.recv");
  if (res < 0) {
    printf("Operation failed!");
  }
}

/**** AUXILIARY FUNCTIONS ****/

/* request_record: Requests the table size, calls ask_id,
 * and then requests a dogType record.
 * If the requested record is valid, it returns its id.
 * aux to cli_view and cli_delete */
int request_record(int socketfd, dogType *p_dog) {
  unsigned table_size, id;
  // Get table size
  if (recv(socketfd, &table_size, sizeof(int), 0) < 0)
    handle_error("client.recv");
  // Request dog by ID
  id = ask_id(table_size);
  send(socketfd, &id, sizeof(int), 0);
  // Receive dog
  if (recv(socketfd, p_dog, sizeof(dogType), 0) < 0)
    handle_error("client.recv");
  // Validate dog
  if (p_dog == NULL) {
    printf("No record with ID %u was found\n", id);
    return -1;
  }
  return id;
}

void send_id(int socketfd, unsigned id) {
  if (send(socketfd, &id, sizeof(unsigned), 0) < 0)
    handle_error("send");
}

/* send_name: Call ask_name and send result through socket
 * aux to cli_search */
void send_name(int socketfd) {
  char *name = ask_name();
  if (send(socketfd, name, sizeof(char) * 32, 0) < 0)
    handle_error("send");
  free(name);
}

/* display_medical_hist: Request a file and open it in a text editor
 * aux to view_record */
void display_medical_hist(int socketfd, unsigned id) {
  char *buff = (char *)malloc(sizeof(char) * 512);
  int confirm = 1; // Confirm editor was closed and file can be sent back
  // Temporary text file the user edits locally before sending it to the server
  char file_name[16];
  sprintf(file_name, "temp/%u.txt", id);
  FILE *temp_file;
  // Try to open file
  if ((temp_file = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");
  // Get data from socket: socket => buffer => file
  relay_file_contents(socketfd, fileno(temp_file), buff);
  // Open file in text editor
  system(sprintf("nano %s", id));
  // After file is edited, send signal to server to resume communications
  if (send(socketfd, &confirm, sizeof(int), 0) < 0)
    handle_error("client.send");
  // Write updated data back to socket: file => buffer => socket
  relay_file_contents(fileno(temp_file), socketfd, buff);
  // Close file
  if (fclose(temp_file) != 0)
    handle_error("fclose");
  free(buff);
}

/**** USER INPUT FUNCTIONS ****/

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
    } // end if
  }   // end while
}

/* ask_name: Returns pointer to string on the heap */
char *ask_name() {
  char *name = malloc(sizeof(char) * 32);
  while (1) { // Name cannot be empty
    printf("Escriba el nombre a buscar: ");
    scanf("%s", name);
    str_lower_case(name);
    if (strcmp(name, "") == 0) {
      printf(INPUT_WARNING);
    } else {
      break;
    }
  }
  return name;
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

/* print_record: */
void print_record(dogType *dog_ptr, unsigned id) {
  printf("record %u\n", id);
  printf("Nombre: %s\n", dog_ptr->name);
  printf("  Tipo: %s\n", dog_ptr->species);
  printf("  Edad: %i\n", dog_ptr->age);
  printf("  Raza: %s\n", dog_ptr->breed);
  printf("Altura: %i\n", dog_ptr->height);
  printf("  Peso: %.3f\n", dog_ptr->weight);
  printf("  Sexo: %c\n", dog_ptr->sex);
}
