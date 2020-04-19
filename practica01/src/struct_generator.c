#include "../p1-dogProgram.h"

int generate_random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
  //return rand() % max; // range [0, max)
}

float generate_random_float(float max) {
  return ((float)rand() / (float)(RAND_MAX)) * max;
}

char *generate_random_string(int len) {
  char *rstr = malloc((len + 1) * sizeof(char));
  for (int i = 0; i < len; i++) {
    rstr[i] = generate_random_int(97, 123); // ASCII a-z
  }
  rstr[len] = '\0';
  return rstr;
}

char generate_random_gender() {
  int rand = generate_random_int(0, 2); // Either 0 or 1
  if (rand == 0) {
    return 'H';
  }
  return 'M';
}

struct dogType generate_dog() {
  struct dogType *dog_ptr = (struct dogType *)malloc(sizeof(struct dogType));

  strcpy(dog_ptr->name, generate_random_string(32));
  strcpy(dog_ptr->species, generate_random_string(32));
  strcpy(dog_ptr->breed, generate_random_string(16));
  dog_ptr->age = generate_random_int(0, 100);
  dog_ptr->height = generate_random_int(0, 100);
  dog_ptr->weight = generate_random_float(100);
  dog_ptr->sex = generate_random_gender();
  dog_ptr->ID = -1; // Hash table will give the dog a proper ID

  return *dog_ptr;
}

Table generate_table() {
  srand(time(NULL));   // Initialization, should only be called once.
  Table ht = create_table();

  for (unsigned long i = 0; i < 10000000; i++) {
    printf("%lu", i);
    struct dogType dog = generate_dog();
    insert_to_table(ht, dog.name, dog);
  }
  return ht;
}
