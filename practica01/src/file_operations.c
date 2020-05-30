#include "../p1-dogProgram.h"

/* poly_hash: Polynomial hash function for strings */
unsigned int poly_hash(char *s) {
  static const unsigned x = 31;
  unsigned int hash_value = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
  }
  return hash_value % NUM_RECORDS; // Hash can't exceed the size of the table
}

/* allocate_record: wrapper around malloc(record_t) */
record_t *allocate_record() {
  record_t *new_record = (record_t *)malloc(sizeof(record_t));
  if (new_record == NULL) {
    perror("ERROR malloc");
    exit(-1);
  }
  return new_record;
}

/* open_table_file: wrapper around malloc(table_t) and fopen */
table_t *open_table_file() {
  table_t *new_table = (table_t *)malloc(sizeof(table_t));
  if (new_table == NULL) {
    perror("ERROR malloc");
    exit(-1);
  } else if ((new_table->fptr = fopen(FILE_NAME, "wb+")) == NULL) {
    perror("ERROR fopen");
    exit(-1);
  }
  new_table->size = 0;
  new_table->capacity = NUM_RECORDS;
  return new_table;
}

/* close_table_file: wrapper around free() and fclose */
void close_table_file(table_t *table) {
  if (fclose(table->fptr) != 0) {
    perror("ERROR fclose");
    exit(-1);
  }
  free(table);
}

/* read_from_table: wrapper around fread */
void read_from_table(table_t *table_ptr, record_t *record_ptr) {
  if (fread(record_ptr, sizeof(record_t), 1, table_ptr->fptr) < 0) {
    perror("ERROR fread");
    exit(-1);
  }
}

/* write_to_table: wrapper around fwrite */
void write_to_table(table_t *table_ptr, record_t *record_ptr) {
  if (fwrite(record_ptr, sizeof(record_t), 1, table_ptr->fptr) <= 0) {
    perror("ERROR fwrite");
    exit(-1);
  }
}

/* lookup_in_table: Wrapper around fseek */
void lookup_in_table(table_t *table_ptr, int position) {
  fseek(table_ptr->fptr, position * sizeof(record_t), SEEK_SET);
}

