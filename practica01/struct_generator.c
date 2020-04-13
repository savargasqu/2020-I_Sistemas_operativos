#include <time.h> 
#include <string.h>
#include "p1-dogProgram.h"

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
const char gender[] = "HM";

int generate_random_int(int min, int max){
  return (rand() % (min - max)) + min; // range [min, max)
}

float generate_random_float(float max) {
  return ((float)rand()/(float)(RAND_MAX)) * max;
}

char *generate_random_string(int len) {
  char *rstr = malloc((len + 1) * sizeof(char));
  for (int i = 0; i < len; i++) {
    rstr[i] = alphabet[generate_random_int(0, strlen(alphabet))];
  }
  rstr[len] = '\0';
  return rstr;
}

char generate_random_gender() {
  int rand = generate_random_int(0, 2);
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

  return *dog_ptr;
}

int main() {
  //srand(time(0));   // Initialization, should only be called once.
  Table ht = create_table();

  for (unsigned int i = 0; i < 1000000; i++) {
    struct dogType dog = generate_dog();
    insert_to_table(ht, dog.name, dog);
  }
  serialize(ht, 'w');
}

