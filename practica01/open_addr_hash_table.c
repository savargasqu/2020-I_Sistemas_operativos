/* First draft for an open addressing hash table
 *
 * TODO:
 * Add a position offset argument to the read/write function
 * Read CLRS section on open addressing hash tables
 *
 * NOTE: Do all modifications on this file
 * When finished:
 * Update header file
 * Keep the same file structure. It'll make easier the 2nd practice easier
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "people.dat"
#define NUM_RECORDS 3

// Use this simpler struct for development. Replace with pet struct later
typedef struct {
  char name[8];
  int age;
} record_t; 

// New table struct
typedef struct {
  FILE *fptr;            // File pointer
  unsigned int size;     // Current number of records stored
  unsigned int capacity; // Number of records that _can_ be stored
} table_t;

// OPEN ADDRESSING HASH TABLE FILE

/* open_file_table: */
table_t *open_file_table() {
  table_t *new_table = (table_t *)malloc(sizeof(table_t));
  if ((new_table->fptr = fopen(FILE_NAME, "wb+")) == NULL) {
    perror("ERROR fopen");
    exit(-1);
  }
  new_table->size = 0;
  new_table->capacity = NUM_RECORDS;

  return new_table;
}

/* close_file_table: */
void close_file_table(table_t *table) {
  if (fclose(table->fptr) != 0) {
    perror("ERROR fclose");
    exit(-1);
  }
  free(table);
}

/* poly_hash: */
// unsigned int poly_hash(char *s) {
//  const unsigned int x = 31;
//  unsigned int hash_value = 0;
//  for (int i = 0; s[i] != '\0'; i++) {
//    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
//  }
//  return hash_value % NUM_RECORDS; // Hash can't exceed the size of the table
//}

/* modify_record: */
void modify_record(record_t *new_record) {
  printf("New record\nname:");
  scanf("%s", new_record->name);
  printf("age:");
  scanf("%d", &new_record->age);
}

/* write_record_to_file */
void write_record_to_file(table_t *table_ptr, record_t *record_ptr) {
  // Get hash
  // fseek
  if (fwrite(record_ptr, sizeof(record_t), 1, table_ptr->fptr) <= 0) {
    perror("ERROR fwrite");
    exit(-1);
  }
  table_ptr->size += 1;
}

/* read_record_from_file: */
void read_record_from_file(table_t *table_ptr, record_t *record_ptr) {
  if (fread(record_ptr, sizeof(record_t), 1, table_ptr->fptr) <= 0) {
    perror("ERROR fread");
    exit(-1);
  }
}

/* print_record: */
void print_record(record_t *record_ptr) {
  printf("record\n");
  printf("name: %s\n", record_ptr->name);
  printf(" age: %d\n", record_ptr->age);
}

int main() {
  int res; // Result. For error handling

  table_t *table = open_file_table(); // Initialize table

  record_t *temp_write_ptr = (record_t *)malloc(sizeof(record_t));

  for (int i = 0; i < NUM_RECORDS; i++) {
    modify_record(temp_write_ptr);
    write_record_to_file(table, temp_write_ptr);
  }

  rewind(table->fptr); // Put file pointer back at the beginning of the file

  record_t *temp_read_ptr = (record_t *)malloc(sizeof(record_t));
  for (int i = 0; i < NUM_RECORDS; i++) {
    read_record_from_file(table, temp_read_ptr);
    print_record(temp_read_ptr);
  }

  free(temp_write_ptr);
  free(temp_read_ptr);

  return 0;
}
