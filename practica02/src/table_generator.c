#include "../p2.h"

/* generate_random_table: Takes a table and fills it with random records */
void generate_random_table(table_t *p_table) {
  srand(time(NULL)); // For randomization. It should only be called once
  unsigned k;
  dogType *temp = (dogType *)malloc(sizeof(dogType));
  for (int i = 0; i < NUM_RECORDS; i++) {
    generate_random_record(temp);
    k = probe_table(p_table, poly_hash(temp->name));
    insert_record(p_table, temp, k);
    printf("%d: %s\n", k, temp->name); // For debugging
  }
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
