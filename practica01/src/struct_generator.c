#include "../p1-dogProgram.h"

#define NUMBER_OF_STRUCTS 1000000

int main() {
  Table ht = generate_table();
  save_table(ht);
  return 0;
}

Table generate_table() {
  srand(time(NULL));   // Initialization, should only be called once.
  Table ht = create_table();

  for (unsigned long i = 0; i < NUMBER_OF_STRUCTS; i++) {
    struct dogType dog = generate_dog();
    unsigned long id = insert_to_table(ht, dog.name, dog);
    printf("%lu: %s\n", id, dog.name);
  }
  return ht;
}

struct dogType generate_dog() {
  struct dogType *dog_ptr = (struct dogType *)malloc(sizeof(struct dogType));
  strcpy(dog_ptr->name, generate_random_string(32));
  strcpy(dog_ptr->species, generate_random_string(32));
  strcpy(dog_ptr->breed, generate_random_string(16));
  dog_ptr->age = generate_random_int(1, 100);
  dog_ptr->height = generate_random_int(1, 100);
  dog_ptr->weight = generate_random_float(100);
  dog_ptr->sex = generate_random_gender();
  dog_ptr->ID = 0; // Hash table will give the dog a proper ID

  return *dog_ptr;
}

int generate_random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min; // range [min, max]
}

float generate_random_float(float max) {
  return ((float)rand() / (float)(RAND_MAX)) * max;
}

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

char generate_random_gender() {
  if (generate_random_int(0, 2) == 0) { // Either 0 or 1
    return 'H';
  }
  return 'M';
}

