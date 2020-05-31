#include "../p1-dogProgram.h"

/*** OPEN ADDRESSING HASH TABLE FILE ***/

int main() {
  table_t *table = open_table_file(); // Initialize table
  generate_random_table(table);       // Fill table
  dogType *temp = allocate_record();

  // Put file pointer back at the beginning of the file
  rewind(table->fptr);
  // Read all the records written to the file
  for (int i = 0; i < NUM_RECORDS; i++) {
    read_from_table(table, temp);
    print_record(temp);
  }
  display_menu(table, temp);
  free(temp);
  return 0;
}

int display_menu(table_t *ht, dogType *temp) {
  int menu_selection = -1;
  char wait;
  unsigned id;
  char *name;
  while (true) {
    system("clear"); // "cls" for windows
    printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
           "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
           "Ingrese el número de una opción: ");
    scanf("%d", &menu_selection);
    switch (menu_selection) {
    case 1: // Ingresar registro
      ask_new_record(temp);
      id = probe_table(ht, poly_hash(name));
      insert_record(ht, temp, id);
      break;
    case 2: // Ver registro
      id = ask_id(ht);
      view_record(ht, temp, id);
      break;
    case 3: // Borrar registro
      id = ask_id(ht);
      delete_record(ht, temp, id);
      break;
    case 4: // Buscar registro
      name = ask_name();
      search_record_name(ht, name, poly_hash(name));
      break;
    case 5: // Salir
      return 0;
      break;
    default:
      // printf(INPUT_WARNING);
      printf("bad option");
      break;
    }
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait); // works as getchar()
  }
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

/**** INSERT ****/

/* insert_record: inserts new_record to table */
int insert_record(table_t *table, dogType *new_record, unsigned position) {
  if (position >= 0) {
    lookup_in_table(table, position); // Set table pointer to the right position
    write_to_table(table, new_record); // Write record to table
    table->size += 1;
    return 0; // If operation was successful
  }
  return -1; // Insert failed
}

/* probe_table: Search for an empty slot in the table */
unsigned probe_table(table_t *table, unsigned start_pos) {
  unsigned limit = NUM_RECORDS;
  dogType *temp = allocate_record();
  lookup_in_table(table, start_pos);
  for (unsigned i = start_pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, "") == 0) {
      // if there's an empty slot, return that position
      free(temp);
      return i;
    }
    if (i == NUM_RECORDS - 1) {
      i = 0;
      limit = start_pos;
    }
  }
  free(temp);
  return -1; // No spot was found/The table is full and needs resizing
}

/* ask_new_record: Reads fields from stdin and assigns them to the new_record */
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

/**** VIEW ****/

/* view_record: */
void view_record(table_t *table, dogType *temp, unsigned id) {
  char selection;
  lookup_in_table(table, id);
  read_from_table(table, temp);
  if (strcmp(temp->name, "") == 0) {
    printf("No hay registro con este ID\n");
  } else {
    print_record(temp);
    printf("Ver historia clínica?[y/n]\n");
    scanf("%c", &selection);
    if (selection == 'y') {
        open_clinical_history(id); // Argument is ID
    } else {
      return;
    }
  }
}

void open_clinical_history(unsigned id) {
  char *file_name;
  FILE *text_file;
  sprintf(file_name, "%u.txt", id); // File name is dog's ID
  text_file = fopen(file_name, "w"); // Write file in binary format
  if (text_file == NULL) {           // If pointer is NULL, fopen failed
    perror(ERR_OPEN);
    exit(-1);
  } else {
    char command[64];
    sprintf(command, "open %s", file_name);
    system(command);
  }
}

/* print_record: */
void print_record(dogType *dog_ptr) {
  printf("record\n");
  printf("Nombre: %s\n", dog_ptr->name);
  printf("  Tipo: %s\n", dog_ptr->species);
  printf("  Edad: %i\n", dog_ptr->age);
  printf("  Raza: %s\n", dog_ptr->breed);
  printf("Altura: %i\n", dog_ptr->height);
  printf("  Peso: %.3f\n", dog_ptr->weight);
  printf("  Sexo: %c\n", dog_ptr->sex);
}

/**** DELETE ****/

/* delete_record: */
void delete_record(table_t *table, dogType *temp, unsigned id) {
  lookup_in_table(table, id);
  // Writing a null struct is equivalent to deleting it
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

/**** SEARCH ****/

/* search_record_name: Search all records with a matching name */
void search_record_name(table_t *table, char *name, unsigned start_pos) {
  unsigned limit = NUM_RECORDS;
  dogType *temp = allocate_record();
  lookup_in_table(table, start_pos);
  for (unsigned i = start_pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, name) == 0) {
      // If a match is found, print it
      print_record(temp);
    } else if ((strcmp(temp->name, "") == 0) && (temp->deleted == false)) {
      // If a field that has never been initialized is found, stop the search
      break;
    }
    if (i == NUM_RECORDS - 1) {
      i = 0;
      limit = start_pos;
    }
  }
  free(temp);
}

char *ask_name() {
  char *name = malloc(sizeof(char) * 32);
  while (strcmp(name, "") == 0) { // Name cannot be empty
    printf("Escriba el nombre a buscar: ");
    scanf("%s", name);
    str_lower_case(name);
  }
  return name;
}

/**** AUXILIARY FUNCTIONS ****/

/* ask_id: Prints the size of the table and reads an integer from stdin */
unsigned ask_id(table_t *table) {
  // TODO check id is valid
  unsigned id; // ID of record
  printf("La tabla tiene %u registros\n", table->size);
  printf("Ingrese el ID de un registro: ");
  scanf("%u", &id);
  return id;
}

void str_lower_case(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}
