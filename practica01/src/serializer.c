#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

void save_table(Table table) {
  FILE *file_ptr; // File to store data in
  int result;     // Return value of I/O operations, for error checking
  struct dogType *temp_dog_record =
      (struct dogType *)malloc(sizeof(struct dogType));

  // Open file
  file_ptr = fopen(DATA_FILE, "wb"); // File to write in binary format
  if (file_ptr == NULL) {            // If pointer is NULL, fopen failed
    perror(OPEN_ERROR);
  }

  // Iterate over th hash table and write all the records to the file
  for (unsigned long i = 0; i <= HASHSIZE; i++) {
    for (Node temp = table->buckets[i]; temp != NULL; temp = temp->next) {
      result = fwrite(temp_dog_record, sizeof(struct dogType), 1, file_ptr);
      // Check if there was an error
      if (result <= 0) {
        perror(SERIALIZE_ERROR); // fread returns the number of items read
      }
    }
  }
  // Close file
  result = fclose(file_ptr);
  if (result < 0) { // fclose returns 0 on success
    perror(CLOSE_ERROR);
  }
}

Table load_table() {
  FILE *file_ptr; // File to store data in
  int result;     // Return value of I/O operations, for error checking
  Table table = (Table)malloc(sizeof(struct table));
  struct dogType *temp_dog_record =
      (struct dogType *)malloc(sizeof(struct dogType));

  // Open file
  file_ptr = fopen(DATA_FILE, "rb"); // File to read in binary format
  if (file_ptr == NULL) {            // If pointer is NULL, fopen failed
    perror(OPEN_ERROR);
  }

  // Read each dogType record from the and insert it to the hash table
  do {
    result = fread(temp_dog_record, sizeof(struct dogType), 1, file_ptr);
    if (result == 1) { // If read was succesful
      insert_to_table(table, temp_dog_record->name, *temp_dog_record);
    }
  } while (result == 1);
  // Check if there was an error
  if (result <= 0) {
    perror(SERIALIZE_ERROR); // fread returns the number of items read
  }

  // Close file
  result = fclose(file_ptr);
  if (result < 0) { // fclose returns 0 on success
    perror(CLOSE_ERROR);
  }
  return table;
}
