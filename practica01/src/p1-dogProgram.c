/* TODO:
 * Add deleted field to the struct
 */

#include "../p1-dogProgram.h"

/*** OPEN ADDRESSING HASH TABLE FILE ***/

int main() {
  table_t *table = open_table_file(); // Initialize table
  generate_random_table(table);       // Fill table
  record_t *temp = allocate_record();

  // Put file pointer back at the beginning of the file
  rewind(table->fptr);
  // Read all the records written to the file
  for (int i = 0; i < NUM_RECORDS - 1; i++) {
    read_from_table(table, temp);
    print_record(temp);
  }
  display_menu(table, temp);
  free(temp);
  return 0;
}

int display_menu(table_t *ht, record_t *temp) {
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
int insert_record(table_t *table, record_t *new_record, unsigned position) {
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
  record_t *temp = allocate_record();
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
void ask_new_record(record_t *new_record) {
  printf("Edit record\nname:");
  scanf("%s", new_record->name);
  str_lower_case(new_record->name);
  printf("age:");
  scanf("%d", &new_record->age);
}

/**** VIEW ****/

/* view_record: */
void view_record(table_t *table, record_t *temp, unsigned id) {
  lookup_in_table(table, id);
  read_from_table(table, temp);
  if (strcmp(temp->name, "") == 0) {
    printf("No hay registro con este ID");
  } else {
    print_record(temp);
    // TODO: Add text file
  }
}

/* print_record: */
void print_record(record_t *record_ptr) {
  printf("record\n");
  printf("name: '%s'\n", record_ptr->name);
  printf(" age:  %d;\n", record_ptr->age);
}

/**** DELETE ****/

/* delete_record: */
void delete_record(table_t *table, record_t *temp, unsigned id) {
  lookup_in_table(table, id);
  read_from_table(table, temp);
  // Writing a null struct is equivalent to deleting it
  strcpy(temp->name, "");
  temp->age = 0;
  temp->deleted = true;
  write_to_table(table, temp);
  table->size -= 1;
}

/**** SEARCH ****/

/* search_record_name: Search all records with a matching name */
void search_record_name(table_t *table, char *name, unsigned start_pos) {
  unsigned limit = NUM_RECORDS;
  record_t *temp = allocate_record();
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
