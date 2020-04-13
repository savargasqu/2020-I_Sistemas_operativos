#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

Table serialize(Table table, char mode) {
  FILE *file_ptr; // File to store data in
  int result;     // Return value of I/O operations, for error checking

  file_ptr = fopen(DATA_FILE, "wb+"); // File to read & write in binary format
  if (file_ptr == NULL) {             // If pointer is NULL, fopen failed
    perror(OPEN_ERROR);
    exit(-1);
  }
  if (mode == 'r') {
    table = create_table();
    // Read the hash table from file
    result = fread(table, sizeof(struct table), 1, file_ptr);
  } else if (mode == 'w') {
    // Write the hash table to file
    result = fwrite(table, sizeof(struct table), 1, file_ptr);
  }
  /* fread and fwrite return the number of items read or written.
   * If it's 0, it didn't write anything */
  if (result <= 0) {
    perror(SERIALIZE_ERROR);
  }
  result = fclose(file_ptr);
  if (result < 0) { // fclose returns 0 on success
    perror(CLOSE_ERROR);
    exit(-1);
  }
  return table;
}

