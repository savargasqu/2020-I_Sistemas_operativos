#include "p1-dogProgram.h"
/* RANDOMIZATION MODULE */
float generate_random_float(float max) {
  return ((float)rand() / (float)(RAND_MAX)) * max;
}

char generate_random_gender() {
  if (generate_random_int(0, 2) == 0) { // Either 0 or 1
    return 'H';
  }
  return 'M';
}

//struct dogType generate_dog() {
//  struct dogType *dog_ptr = (struct dogType *)malloc(sizeof(struct dogType));
//  strcpy(dog_ptr->name, generate_random_string(32));
//  strcpy(dog_ptr->species, generate_random_string(32));
//  strcpy(dog_ptr->breed, generate_random_string(16));
//  dog_ptr->age = generate_random_int(1, 100);
//  dog_ptr->height = generate_random_int(1, 100);
//  dog_ptr->weight = generate_random_float(100);
//  dog_ptr->sex = generate_random_gender();
//  dog_ptr->ID = 0; // Hash table will give the dog a proper ID
//
//  return *dog_ptr;
//}

