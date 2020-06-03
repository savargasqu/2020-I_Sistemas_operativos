// DON'T DELETE. THREADS ARE HERE
#include "./include/p2-shared.h"

#define NUM_THREADS 5
#define ERR_THREAD "Error en pthread_create. No se pudo crear un hilo con éxito"

/*** OPEN ADDRESSING HASH TABLE FILE ***/

int main() {
  table_t *table = open_table_file(); // Initialize table
  generate_random_table(table);       // Fill table

  pthread_t thread[NUM_THREADS]; // Array of threads
  int res, i = 0;
  while (true) {
    if (i <
        NUM_THREADS) { //&& signal) { -- wait for client signal to create new
      if (pthread_create(&thread[i], NULL, (void *)display_menu, table) != 0) {
        perror(ERR_THREAD);
        exit(-1);
      } else {
        pthread_join(thread[i], (void **)&res);
        i++;
      }
    }
  }
  printf("back to main");
  return 0;
}

void display_menu(table_t *ht) {
  dogType *temp = allocate_record();
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
      free(temp);
      return;
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


/* view_record: */
void view_record(table_t *table, dogType *temp, unsigned id) {
  char selection = '0';
  lookup_in_table(table, id);
  read_from_table(table, temp);
  if (strcmp(temp->name, "") == 0) {
    printf("No hay registro con este ID\n");
  } else {
    print_record(temp, id);
    printf("Ver historia clínica?[y/n]\n");
    scanf("%c", &selection);
    if (selection == 'y') {
      open_clinical_history(id); // Argument is ID
    }
  }
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


/* search_record_name: Search all records with a matching name */
void search_record_name(table_t *table, char *name, unsigned start_pos) {
  unsigned limit = NUM_RECORDS;
  dogType *temp = allocate_record();
  lookup_in_table(table, start_pos);
  for (unsigned i = start_pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, name) == 0) {
      // If a match is found, print it
      print_record(temp, i);
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

