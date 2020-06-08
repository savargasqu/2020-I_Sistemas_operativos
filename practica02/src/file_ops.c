#include "../p2.h"

/* File-level functions i.e. wrappers for file system calls */

/* open_table_file: Wrapper around malloc(table_t) and fopen */
table_t *open_table_file(bool new) {
  table_t *new_ht = (table_t *)malloc(sizeof(table_t));
  if (new_ht == NULL)
    handle_error("malloc");
  // Open file
  if ((new_ht->fptr = fopen(DATA_FILE, "wb+")) == NULL)
    handle_error("fopen");
  // Initialize table values
  // if (!new) { // Recover size of table
  //  rewind(new_ht->fptr);
  //  if (fread(&new_ht->size, sizeof(unsigned), 1, new_ht->fptr) < 0)
  //    handle_error("read");
  //}
  new_ht->size = GENERATED_RECORDS;
  new_ht->capacity = NUM_RECORDS;
  // printf("Table opens with %u records\n", new_ht->size); // For debugging
  return new_ht;
}

/* close_table_file: Wrapper around free() and fclose */
void close_table_file(table_t *table) {
  printf("table closes with %u records\n", table->size);
  //// Save the size of the table
  // rewind(table->fptr);
  // if (fwrite(&table->size, sizeof(unsigned), 1, table->fptr) < 0)
  //  handle_error("write");
  if (fclose(table->fptr) != 0)
    handle_error("fclose");

  free(table);
}

/* lookup_in_table: Wrapper around fseek */
void lookup_in_table(table_t *p_table, unsigned pos) {
  // if (fseek(p_table->fptr, (pos * sizeof(dogType)) + sizeof(unsigned),
  // SEEK_SET) != 0) {
  if (fseek(p_table->fptr, pos * sizeof(dogType), SEEK_SET) != 0) {
    handle_error("seek");
  }
}

/* read_from_table: Wrapper around fread */
void read_from_table(table_t *p_table, dogType *p_record) {
  if (fread(p_record, sizeof(dogType), 1, p_table->fptr) < 0)
    handle_error("read");
}

/* write_to_table: Wrapper around fwrite */
void write_to_table(table_t *p_table, dogType *p_record) {
  if (fwrite(p_record, sizeof(dogType), 1, p_table->fptr) <= 0)
    handle_error("write");
}

/* LOG OPERATIONS */
FILE *open_log() {
  FILE *p_log;
  if ((p_log = fopen(LOG_FILE, "a")) == NULL) // Open in append mode
    handle_error("read");
  return p_log;
}

void close_log(FILE *p_log) {
  if (fclose(p_log) != 0)
    handle_error("close");
}

// [TIME] [IP] [OPERATION] [ID/NAME]
void write_to_log(FILE *p_log, char *op, char *name, unsigned id, char *IP) {
  time_t timestamp;
  struct tm *time_struct;
  char time_buf[24];
  char *log_entry = (char *)malloc(sizeof(char) * 160);
  /* Format time */
  // Place epoch time in timestamp
  time(&timestamp);
  // Turn epoch time in human-readable time
  time_struct = localtime(&timestamp);
  // [YYYYMMDDTHHMMSS]
  strftime(time_buf, 24, "%Y%m%dT%H%M%S", time_struct);
  // Format the rest
  if (strcmp(op, "search") == 0) {
    sprintf(log_entry, "[%s] [%s] [%s] [%s]\n", time_buf, IP, op, name);
  } else {
    sprintf(log_entry, "[%s] [%s] [%s] [%u]\n", time_buf, IP, op, id);
  }
  printf("LOG: %s", log_entry); // For debugging
  if (fputs(log_entry, p_log) <= 0)
    handle_error("write");
}
