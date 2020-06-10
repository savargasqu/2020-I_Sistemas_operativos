#include "../p2.h"

int main() {
  int socketfd, rv;
  int numbytes;
  struct addrinfo hints, *servinfo, *res;
  /* Write hints for the getaddrinfo function */
  memset(&hints, 0, sizeof(hints)); // Fill the struct with 0s before beginning
  hints.ai_family = AF_UNSPEC;      // Let the socket use either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // Stream socket (TCP)
  if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
    handle_error("server.getaddrinfo");

  // Create a new socket
  socketfd =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if (socketfd < 0)
    handle_error("socket");
  // Connect to the server
  if (connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
    handle_error("connect");
  } else {
    printf("Connected to server...\n");
  }
  if (servinfo == NULL) {
    fprintf(stderr, "Falla de conexión\n");
  }

  cli_menu(socketfd);

  if (close(socketfd) != 0)
    handle_error("close");
  return 0;
}

/**** CLIENT SIDE ****/

void cli_menu(int clientfd) {
  int menu_option = -1;
  char wait;
  while (true) {
    system("clear");
    printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
           "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
           "Ingrese el número de una opción: ");
    // Read user input
    scanf("%d", &menu_option);
    // Tell the server what kind of operation is going to be performed
    if (0 < menu_option && menu_option <= 5) {
      if (send(clientfd, &menu_option, sizeof(int), 0) < 0)
        handle_error("client.send");
    } else {
      printf(INPUT_WARNING);
      continue;
    }
    switch (menu_option) {
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
    } // end switch
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait);
  } // end while
}

void cli_insert(int socketfd) {
  int id;
  dogType *temp = allocate_record();
  ask_new_record(temp);
  send_record(socketfd, temp);
  id = recv_id(socketfd);
  if (id != NUM_RECORDS + 1) {
    printf("Registro creado con con ID %d\n", id);
  } else {
    printf("Registro no fue creado exitosamente\n");
  }
  free(temp);
}

void cli_view(int socketfd) {
  int table_size, id;
  bool found, ok = false; // If user wants to open file
  dogType *temp = allocate_record();
  // Send id
  table_size = recv_id(socketfd); // We can use recv_id to get table_size
  id = ask_id(table_size);
  send_id(socketfd, id);
  // Check if found
  if (recv(socketfd, &found, sizeof(bool), 0) < 0)
    handle_error("server.send");
  if (!found) {
    printf("No se encontró ningún registro\n");
    return;
  }
  // If a record was found
  recv_record(socketfd, temp);
  print_record(temp, id);
  // Let the server know whether the file will be opened or not
  ok = ask_open_hist();
  send(socketfd, &ok, sizeof(bool), 0);
  if (ok) {
    open_medical_hist(socketfd, id);
  }
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
  unsigned id;
  dogType *temp = allocate_record();
  char *name = ask_name();
  send_name(socketfd,
            name); // Ask user for a namea, and send it to the server
  while (true) {   // Print until there aren't any more records being send
    id = recv_id(socketfd);
    recv_record(socketfd, temp);
    if (id >= NUM_RECORDS) {
      break;
    } else {
      print_record(temp, id);
    } // end if
  }   // end while
}

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
  bool confirm = true; // Confirm editor was closed and file can be sent back
  // text file the user edits locally before sending it to the server
  FILE *fp;
  char *file_name = (char *)malloc(sizeof(char) * 9);
  file_name = "temp.txt"; // Name of file doesn't matter for client
  // Try to open file
  if ((fp = fopen(file_name, "w+")) == NULL)
    handle_error("fopen");
  // Get data from socket: socket => buffer => file
  recv_file(socketfd, file_name);
  // relay_file_contents(socketfd, fileno(fp));
  // Open file in text editor
  system("nano ./temp.txt");
  // Send confirmation to server to resume communications
  if (send(socketfd, &confirm, sizeof(bool), 0) < 0)
    handle_error("client.send");
  // Write updated data back to socket: file => buffer => socket
  send_file(socketfd, file_name);
  // relay_file_contents(fileno(fp), socketfd);
  // Close file
  if (fclose(fp) != 0)
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
    str_upper_case(name);
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
