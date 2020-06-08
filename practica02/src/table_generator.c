#include "../p2.h"

//int main() {
//  // Hash table file
//  table_t *table = open_table_file(true); // Initialize table
//  generate_random_table(table);           // Fill table
//  close_table_file(table);
//  return 0;
//}

/* generate_random_table: Takes a table and fills it with random records */
void generate_random_table(table_t *p_table) {
  srand(time(NULL)); // For randomization. It should only be called once
  dogType *temp = (dogType *)malloc(sizeof(dogType));
  unsigned i, k, c = 0;
  //lookup_in_table(p_table, 0);
  //for (i = 0; i < NUM_RECORDS; i++) { // Wipe previous table
  //  delete_record(p_table, temp, i);
  //}
  p_table->size = 0; // Force table size back to zero
  for (i = 0; i < GENERATED_RECORDS; i++) {
    generate_random_record(temp);
    k = probe_table(p_table, poly_hash(temp->name));
    if (insert_record(p_table, temp, k)) {
      //printf("%d: %s\n", k, temp->name); // For debugging
      c++;
    }
  }
  //printf("%u records were inserted\n", c);
}

/* generate_random_record: */
void generate_random_record(dogType *p_dog) {
  strcpy(p_dog->name, generate_random_string(32));
  strcpy(p_dog->species, generate_random_string(32));
  strcpy(p_dog->breed, generate_random_string(16));
  p_dog->age = generate_random_int(1, 100);
  p_dog->height = generate_random_int(1, 100);
  p_dog->weight = generate_random_float(100);
  p_dog->sex = generate_random_gender();
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

float generate_random_float(float max) {
  return ((float)rand() / (float)(RAND_MAX)) * max;
}

char generate_random_gender() {
  if (generate_random_int(0, 2) == 0) { // Either 0 or 1
    return 'H';
  }
  return 'M';
}
