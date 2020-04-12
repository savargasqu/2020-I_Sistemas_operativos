#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

/* TODO: 
 * Join with hash function implementation
 * Implement search and delete front-end
 */

/* request_data: takes a dogType reference and writes the data input from user*/
void request_data(struct dogType *dog_entry) {
  /* TODO:
   * Add error handling
   * Make entries lower case
   */
  fflush(stdin);
  printf("Por favor ingrese los datos solicitados.\n");
  printf("\nNombre: ");
  scanf("%s", dog_entry->name);
  printf("\nTipo: ");
  scanf("%s", dog_entry->species);
  printf("\nEdad (en años): ");
  scanf("%i", &dog_entry->age);
  printf("\nRaza: ");
  scanf("%s", dog_entry->breed);
  printf("\nEstatura (cm): ");
  scanf("%i", &dog_entry->height);
  printf("\nPeso (Kg): ");
  scanf("%f", &dog_entry->weight);
  printf("\nSexo (H/M): ");
  scanf(" %c", &dog_entry->sex);
}

/*create_entry: Create a dogType struct and writes it to a bin file*/
struct dogType *create_entry(struct dogType *dog_entry) {
  FILE *write_file; // File to store data in
  int result;       // Return value of I/O operations, for error checking

  // Create instance of dogType
  dog_entry = (struct dogType *)malloc(sizeof(struct dogType));
  if (dog_entry == NULL) {
    perror(MEMORY_ERROR);
    exit(-1);
  }

  request_data(dog_entry); // Write the necessary information in the structure

  // Instance file and write the results
  write_file = fopen("dataDogs.dat", "wb"); // Write file in binary format
  if (write_file == NULL) {                 // If pointer is NULL, fopen failed
    perror(OPEN_ERROR);
    exit(-1);
  }
  result = fwrite(dog_entry, sizeof(struct dogType), 1, write_file);
  // Returns the number of items written. If it's 0, it didn't write anything
  if (result <= 0) {
    perror(WRITE_ERROR);
    exit(-1);
  }
  result = fclose(write_file);
  if (result < 0) { // fclose returns 0 on success
    perror(CLOSE_ERROR);
    exit(-1);
  }
  return dog_entry;
}

/* view_entry: print the information of the requested dogType */
void view_entry(struct dogType *data) {
  /* TODO:
   * Show number of existing entries
   * Check ID exists
   */
  int view_selection;
  printf("Selección: Ver registro\n1. Ver datos\n2. Ver historia clínica\n");
  scanf("%d", &view_selection);
  while(1) {
    if (view_selection == 1) {
      display_dog_data();
      break;
    } else if (view_selection == 2) {
      edit_clinical_history();
      break;
    } else {
      printf(INPUT_WARNING);
    }
  }
}

/* display_dog_data: Print the information of the requested dogType */
void display_dog_data() {
  struct dogType dog_readBuffer;
  FILE *read_file;
  int r;
  // TODO: Request ID
  read_file = fopen("dataDogs.dat", "rb");
  if (read_file == NULL) {
    perror(READ_ERROR);
    exit(-1);
  }
  // Read
  fread(&dog_readBuffer, sizeof(struct dogType), 1, read_file);

  printf("Nombre: %s\n", dog_readBuffer.name);
  printf("  Tipo: %s\n", dog_readBuffer.species);
  printf("  Edad: %i\n", dog_readBuffer.age);
  printf("  Raza: %s\n", dog_readBuffer.breed);
  printf("Altura: %i\n", dog_readBuffer.height);
  printf("  Peso: %f\n", dog_readBuffer.weight);
  printf("  Sexo: %c\n", dog_readBuffer.sex);

  r = fclose(read_file);
  if (r < 0) {
    perror(CLOSE_ERROR);
    exit(-1);
  }
}

/* edit_clinical_history: Opens the dog's clinical history in a text file */
void edit_clinical_history() {
  // TODO: Juntar código de Tatiana

}


int menu() {
  struct dogType *dog_entry; // Pointer to structure
  int menu_selection = -1;
  char wait;

  while (1) {
    system("clear");
    printf("Menu:\n1. Ingresar Registro\n2. Ver Registro\n3. Buscar Registro"
           "\n4. Borrar Registro\n5. Salir\n"
           "Ingrese el número de opción que desea utilizar: ");
    scanf("%d", &menu_selection);

    switch (menu_selection) {
    case 1: // Ingresar registro
      create_entry(dog_entry);
      break;
    case 2: // Ver registro
      view_entry(dog_entry);
      break;
    case 3: // Borrar registro
      printf("sin implementar.");
      break;
    case 4: // Buscar registro
      printf("sin implementar.");
      break;
    case 5: // Salir
      return 0;
      break;
    default:
      printf(INPUT_WARNING);
      break;
    }
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait); // works as getchar()
  }
  free(dog_entry);
}

/*main: Calls menu*/
int main() {
  // preload hash table
  return menu();
}
