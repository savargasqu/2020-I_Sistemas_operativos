#include "../p1-dogProgram.h"

/* Randomization module. Functions that generate different random data types */

/* generate_random_table: Takes a table and fills it with random records */
void generate_random_table(table_t *table_ptr) {
  srand(time(NULL)); // For randomization. It should only be called once
  unsigned k;
  record_t *temp = (record_t *)malloc(sizeof(record_t));
  for (int i = 0; i < NUM_RECORDS - 1; i++) {
    generate_random_record(temp);
    k = search_record(table_ptr, poly_hash(temp->name), "");
    insert_record(table_ptr, temp, k);
    printf("%d: %s\n", k, temp->name); // For debugging
  }
}

/* generate_random_record: */
void generate_random_record(record_t *record) {
  strcpy(record->name, generate_random_string(8));
  record->age = generate_random_int(1, 100);
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

//----------------------------------------
float generate_random_float(float max) {
  return ((float)rand() / (float)(RAND_MAX)) * max;
}

char generate_random_gender() {
  if (generate_random_int(0, 2) == 0) { // Either 0 or 1
    return 'H';
  }
  return 'M';
}

/*
dogType generate_dog() {
  dogType *dog_ptr = allocate_record();
  strcpy(dog_ptr->name, generate_random_string(32));
  strcpy(dog_ptr->species, generate_random_string(32));
  strcpy(dog_ptr->breed, generate_random_string(16));
  dog_ptr->age = generate_random_int(1, 100);
  dog_ptr->height = generate_random_int(1, 100);
  dog_ptr->weight = generate_random_float(100);
  dog_ptr->sex = generate_random_gender();
  //dog_ptr->ID = 0; // This isn't valid any more

  return *dog_ptr;
}
*/
