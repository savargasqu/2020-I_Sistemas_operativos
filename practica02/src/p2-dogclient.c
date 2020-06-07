#include "../p2.h"

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

/**** CLIENT SIDE ****/

void cli_menu(int clientfd) {
  int id, menu_selection = -1;
  char wait;
  while (true) {
    system("clear");
    printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
           "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
           "Ingrese el número de una opción: ");
    // Read user input
    scanf("%d", &menu_selection);
    // Tell the server what kind of operation is going to be performed
    send(clientfd, &menu_selection, sizeof(int), 0);
    switch (menu_selection) {
    case 1: // Ingresar registro
      cli_insert(clientfd);
      break;
    case 2: // Ver registro
      cli_view(clientfd);
      break;
    case 3: // Borrar registro
      cli_delete(clientfd);
      break;
    case 4: // Buscar registro
      cli_search(clientfd);
      break;
    case 5: // Salir
      return;
    default:
      printf(INPUT_WARNING);
      break;
    }
    cli_confirm_op(clientfd);
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait);
  }
}

void cli_insert(int socketfd) {
  dogType *temp = allocate_record();
  ask_new_record(temp);
  send_record(socketfd, temp);
  free(temp);
}

void cli_view(int socketfd) {
  int table_size, id;
  bool ok = false;
  dogType *temp = allocate_record();
  table_size = recv_id(socketfd); // We can use recv_id to get table_size
  id = ask_id(table_size);
  send_id(socketfd, id);
  recv_record(socketfd, temp);
  // If a record was found
  if (temp != NULL) {
    print_record(temp, id);
    ok = ask_open_hist();
    // Let the server know whether the file will be opened or not
    send(socketfd, &ok, sizeof(bool), 0);
    if (ok) {
      open_medical_hist(socketfd, id);
    }
  } else {
    send(socketfd, &ok, sizeof(bool), 0);
    printf("No se encontró ningún registro");
  } // end if
  free(temp);
}

void cli_delete(int socketfd) {
  int table_size, id;
  dogType *temp = allocate_record();
  // recv_id takes an unsigned, so we can also use it to get the table size
  table_size = recv_id(socketfd);
  id = ask_id(table_size);
  send_id(socketfd, id);
  free(temp);
}

void cli_search(int socketfd) {
  int id;
  dogType *temp = allocate_record();
  send_name(socketfd); // Ask the user for a name and send it to the server.
  while (true) {       // Print until there aren't any more records being send
    recv_record(socketfd, temp);
    recv_id(socketfd);
    if (temp == NULL) {
      break;
    } else {
      print_record(temp, id);
    } // end if
  }   // end while
}

//void cli_confirm_op(int socketfd) {
//  bool ok;
//  if (recv(socketfd, &ok, sizeof(bool), 0) < 0)
//    handle_error("client.recv");
//  if (!ok) {
//    printf("Operation failed!");
//  }
//}

/**** AUXILIARY AND I/O FUNCTIONS ****/

/* ask_open_hist: Ask user (for a char) and returns a bool
 * aux to view_record */
bool ask_open_hist(int socketfd, unsigned id) {
  char ans = '\0';
  while (true) {
    printf("\nAbrir historial médico? [Y/n] ");
    scanf(" %c", &ans);
    if (ans == 'Y' || ans == 'y') {
      return true;
    } else if (ans == 'N' || ans == 'n') {
      return false;
    } else {
      printf(INPUT_WARNING);
    } // end if: question
  }   // end while
}

/* display_medical_hist: Request a file and open it in a text editor
 * aux to view_record */
void open_medical_hist(int socketfd, unsigned id) {
  int confirm = 1; // Confirm editor was closed and file can be sent back
  // Temporary text file the user edits locally before sending it to the server
  char file_name[16];
  sprintf(file_name, "temp/%u.txt", id);
  FILE *temp_file;
  // Try to open file
  if ((temp_file = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");
  // Get data from socket: socket => buffer => file
  relay_file_contents(socketfd, fileno(temp_file));
  // Open file in text editor
  system(sprintf("nano %s", id));
  // After file is edited, send signal to server to resume communications
  if (send(socketfd, &confirm, sizeof(int), 0) < 0)
    handle_error("client.send");
  // Write updated data back to socket: file => buffer => socket
  relay_file_contents(fileno(temp_file), socketfd);
  // Close file
  if (fclose(temp_file) != 0)
    handle_error("fclose");
}

/* ask_id: Prints the size of the table and reads an integer from stdin */
unsigned ask_id(unsigned table_size) {
  unsigned id;
  printf("La tabla tiene %u registros\n", table_size);
  while (true) {
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

/* ask_new_record: Reads fields from stdin and assigns them to a new_record
 * aux to insert_record */
void ask_new_record(dogType *dog_record) {
  fflush(stdin);
  printf("Por favor ingrese los datos solicitados.\n");
  printf("Nombre: ");
  scanf("%s", dog_record->name);
  str_upper_case(dog_record->name);
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

/* str_upper_case: aux to ask_new_record */
void str_upper_case(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper(str[i]);
  }
}

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
