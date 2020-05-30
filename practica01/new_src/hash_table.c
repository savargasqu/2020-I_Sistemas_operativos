/* TODO:
 * Collision handling (in insert)
 * Add deleted field to the struct
 * Loop back on search
 * NOTE: Do all modifications on this file. When finished:
 * Update header file
 * Keep the same file structure. It'll make the 2nd practice easier
 */

/*** MODULE: OPEN ADDRESSING HASH TABLE FILE ***/
#include "hash_table.h"

/* poly_hash: */
unsigned int poly_hash(char *s) {
  static const unsigned x = 31;
  unsigned int hash_value = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
  }
  return hash_value % NUM_RECORDS; // Hash can't exceed the size of the table
}
/* open_table_file: */
table_t *open_table_file() {
  table_t *new_table = (table_t *)malloc(sizeof(table_t));
  if ((new_table->fptr = fopen(FILE_NAME, "wb+")) == NULL) {
    perror("ERROR fopen");
    exit(-1);
  }
  new_table->size = 0;
  new_table->capacity = NUM_RECORDS;
  return new_table;
}
/* close_table_file: */
void close_table_file(table_t *table) {
  if (fclose(table->fptr) != 0) {
    perror("ERROR fclose");
    exit(-1);
  }
  free(table);
}
/* read_record: wrapper around fread */
void read_record(table_t *table_ptr, record_t *record_ptr) {
  if (fread(record_ptr, sizeof(record_t), 1, table_ptr->fptr) < 0) {
    perror("ERROR fread");
    exit(-1);
  }
}
/* write_record: wrapper around fwrite */
void write_record(table_t *table_ptr, record_t *record_ptr) {
  if (fwrite(record_ptr, sizeof(record_t), 1, table_ptr->fptr) <= 0) {
    perror("ERROR fwrite");
    exit(-1);
  }
  table_ptr->size += 1; // If the operation was successful, increase table size
}
/* lookup_record: Wrapper around fseek */
void lookup_record(table_t *table_ptr, int position) {
  fseek(table_ptr->fptr, position * sizeof(record_t), SEEK_SET);
}

/**** MENU OPERATIONS ****/

/* insert_record: inserts new_record to table */
int insert_record(table_t *table, record_t *new_record) {
  // Computes hash and calls probe function
  int pos = search_record(table, poly_hash(new_record->name), "");
  if (pos >= 0) {
    lookup_record(table, pos); // Set table pointer to the correct position
    write_record(table, new_record); // Write record to table
    return pos;                      // Return place where record was written/ID
  }
  return -1; // Insert failed
}

void view_record(table_t *table, record_t *temp, int id) {
  lookup_record(table, id);
  read_record(table, temp);
  if (strcmp(temp->name, "") == 0) {
    printf("No hay registro con este ID");
  } else {
    print_record(temp);
    // TODO: Add text file
  }
}

void delete_record(table_t *table, record_t *temp, int id) {
  lookup_record(table, id);
  read_record(table, temp);
  // Writing a null struct is equivalent to deleting it
  strcpy(temp->name, "");
  temp->age = 0;
  write_record(table, temp);
}

int search_record(table_t *table, int position, char *name) {
  record_t *temp = (record_t *)malloc(sizeof(record_t)); // Allocate a struct
  int i;
  // Move over to the position where the search begins (hash value)
  lookup_record(table, position);
  // iterate over the table starting from position
  for (i = position; i < NUM_RECORDS - 1; i++) {
    read_record(table, temp);
    if (strcmp(temp->name, "") == 0) { // if there's no record on the spot
      free(temp);
      return i; // We found an empty slot
    } else if (strcmp(temp->name, name) == 0) {
      // if name coincides print ID and record
      printf("ID: %d ", i);
      print_record(temp);
    }
  }
  free(temp);
  return -1;
}

/* ask_id: Prints the size of the table and reads an integer from stdin */
unsigned ask_id(table_t *table) {
  unsigned id; // ID of record
  printf("La tabla tiene %u registros\n", table->size);
  printf("Ingrese el ID de un registro: ");
  scanf("%u", &id);
  return id;
}

/* ask_new_record: Reads fields from stdin and assigns it in the new_record */
void ask_new_record(record_t *new_record) {
  printf("Edit record\nname:");
  scanf("%s", new_record->name);
  printf("age:");
  scanf("%d", &new_record->age);
}

void str_lower_case(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}

char *ask_new_name() {
  char *name = malloc(sizeof(char) * 32);
  while (strcmp(name, "") == 0) { // Name cannot be empty
    printf("Escriba el nombre a buscar: ");
    scanf("%s", name);
    str_lower_case(name);
  }
  return name;
}

/* print_record: */
void print_record(record_t *record_ptr) {
  printf("record\n");
  printf("name: '%s'\n", record_ptr->name);
  printf(" age:  %d;\n", record_ptr->age);
}

/* TEST MAIN */
int main() {
  table_t *table = open_table_file(); // Initialize table
  generate_random_table(table);       // Fill table
  int res;                            // Result. For error handling

  // Read all the records written to the file
  record_t *temp = (record_t *)malloc(sizeof(record_t)); // Allocate a struct
  rewind(table->fptr); // Put file pointer back at the beginning of the file
  for (int i = 0; i < NUM_RECORDS - 1; i++) {
    read_record(table, temp);
    print_record(temp);
  }

  // enter 'repl'
  unsigned k;
  char *name;
  while (true) {
    name = ask_new_name();
    k = poly_hash(name);
    search_record(table, k, name);
  }

  free(temp);
  return 0;
}

/* RANDOMIZATION MODULE */

/* generate_random_table: Takes a table and fills it with random records */
void generate_random_table(table_t *table_ptr) {
  srand(time(NULL)); // For randomization. It should only be called once
  unsigned k;
  record_t *temp = (record_t *)malloc(sizeof(record_t));
  for (int i = 0; i < NUM_RECORDS - 1; i++) {
    generate_random_record(temp);
    k = insert_record(table_ptr, temp);
    printf("%d: %s\n", k, temp->name); // For debugging
  }
}

/* generate_random_int: */
int generate_random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min; // range [min, max]
}

/* generate_random_string: */
char *generate_random_string(int str_len) {
  char *rstr = malloc((str_len) * sizeof(char));
  int name_len = generate_random_int(1, str_len - 2);
  int i;
  for (i = 0; i < name_len; i++) {
    rstr[i] = generate_random_int(97, 122); // ASCII a-z
  }
  rstr[str_len - 1] = '\0';
  return rstr;
}

/* generate_random_record: */
void generate_random_record(record_t *record) {
  strcpy(record->name, generate_random_string(8));
  record->age = generate_random_int(1, 100);
}
