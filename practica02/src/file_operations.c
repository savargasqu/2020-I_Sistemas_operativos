#include "../p1-dogProgram.h"

/* File-level functions i.e. wrappers for system calls that handle errors */

/* allocate_record: Wrapper around malloc(dogType) */
dogType *allocate_record() {
  dogType *new_record = (dogType *)malloc(sizeof(dogType));
  if (new_record == NULL) {
    perror(ERR_MEMORY);
    exit(-1);
  }
  return new_record;
}

/* open_table_file: Wrapper around malloc(table_t) and fopen */
table_t *open_table_file() {
  table_t *new_table = (table_t *)malloc(sizeof(table_t));
  if (new_table == NULL) {
    perror(ERR_MEMORY);
    exit(-1);
  } else if ((new_table->fptr = fopen(FILE_NAME, "wb+")) == NULL) {
    perror(ERR_OPEN);
    exit(-1);
  }
  // Initialize table values
  new_table->size = 0;
  new_table->capacity = NUM_RECORDS;
  return new_table;
}

/* close_table_file: Wrapper around free() and fclose */
void close_table_file(table_t *table) {
  if (fclose(table->fptr) != 0) {
    perror(ERR_CLOSE);
    exit(-1);
  }
  free(table);
}

/* lookup_in_table: Wrapper around fseek */
void lookup_in_table(table_t *table_ptr, unsigned position) {
  if (fseek(table_ptr->fptr, position * sizeof(dogType), SEEK_SET) != 0) {
    perror(ERR_SEEK);
    exit(-1);
  }
}

/* read_from_table: Wrapper around fread */
void read_from_table(table_t *table_ptr, dogType *record_ptr) {
  if (fread(record_ptr, sizeof(dogType), 1, table_ptr->fptr) < 0) {
    perror(ERR_READ);
    exit(-1);
  }
}

/* write_to_table: Wrapper around fwrite */
void write_to_table(table_t *table_ptr, dogType *record_ptr) {
  if (fwrite(record_ptr, sizeof(dogType), 1, table_ptr->fptr) <= 0) {
    perror(ERR_WRITE);
    exit(-1);
  }
}


/* Log operations */

FILE *open_log() {
  FILE *log_ptr;
  if ((log_ptr = fopen(LOG_FILE_NAME, "a")) == NULL) {
    perror(ERR_OPEN);
    exit(-1);
  }
  return log_ptr;

}

void close_log(FILE *log_ptr) {
  if (fclose(log_ptr) != 0) {
    perror(ERR_CLOSE);
    exit(-1);
  }
}

// [TIME] [IP] [OPERATION] [ID/NAME]
void write_to_log(FILE *log_ptr, char *operation, char *name, unsigned id, unsigned IP) {
  time_t timestamp;
  struct tm *time_struct;
  char time_buf[18];
  char log_entry[80];

  // Format time
  time(&timestamp); // Place epoch time in timestamp
  time_struct = localtime(&timestamp); // Turn epoch time in human-readable time
  strftime(time_buf, 18, "[%Y%m%dT%H%M%S] ", time_struct); // [YYYYMMDDTHHMMSS]

  // Format the rest
  if (strcmp(name, "") == 0) {
    sprintf(log_entry, "[%s] [%u] [%s] [%u]", time_buf, IP, operation, id); 
  } else {
    sprintf(log_entry, "[%s] [%u] [%s] [%s]", time_buf, IP, operation, name); 
  }

  if (fwrite(log_entry, sizeof(char)*80, 1, log_ptr) <= 0) {
    perror(ERR_WRITE);
    exit(-1);
  }
}