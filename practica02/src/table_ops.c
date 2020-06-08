#include "../p2.h"

/* poly_hash: Polynomial hash function for strings */
unsigned poly_hash(char *s) {
  static const unsigned x = 31;
  unsigned int hash_value = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
  }
  return hash_value % NUM_RECORDS; // Hash can't exceed the size of the table
}

/* insert_record: inserts new_record to table. Returns true on success */
bool insert_record(table_t *table, dogType *new_record, unsigned pos) {
  if (0 <= pos && pos < NUM_RECORDS) {
    lookup_in_table(table, pos); // Set table pointer to the right position
    write_to_table(table, new_record); // Write record to table
    table->size += 1;
    return true; // If operation was successful
  }
  return false; // Insert failed
}

/* view_record: */
bool view_record(table_t *table, dogType *temp, unsigned id) {
  lookup_in_table(table, id);
  read_from_table(table, temp);
  if (strcmp(temp->name, "") == 0) {
    return false;
  } else {
    return true;
  }
}

/* probe_table: Search for an empty slot in the table
 * auxiliary to insert_record */
unsigned probe_table(table_t *table, unsigned pos) {
  unsigned limit = NUM_RECORDS;
  dogType *temp = allocate_record();
  lookup_in_table(table, pos);
  for (unsigned i = pos; i < limit; i++) {
    read_from_table(table, temp);
    if (strcmp(temp->name, "") == 0) {
      // if there's an empty slot, return that position
      free(temp);
      return i;
    } // if empty
    if (i == NUM_RECORDS - 1) {
      i = 0;
      lookup_in_table(table, 0);
      limit = pos;
    } // if reached end
  } // end for
  free(temp);
  // Else, no spot was found/The table is full and needs resizing
  return NUM_RECORDS + 1;
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
