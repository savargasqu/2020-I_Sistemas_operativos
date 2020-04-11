#include "../hash_table.h"

// For debugging
void print_list(Node head) {
  printf("[");
  for (Node temp = head; temp != NULL; temp = temp->next) {
    printf("%d ", temp->value);
  }
  printf("]\n");
}

void test_search(Table table, int len_values, char *key, int value) {
  Node test = search_in_table(table, key, value);
  if (test != NULL) {
    printf("Found %d\n", test->value);
  } else {
    printf("Didn't found %d\n", value);
  }
}

int main(int argc, char *argv[]) {
  unsigned long h;
  char *test_keys[8];
  test_keys[0] = "rojo";
  test_keys[1] = "naranja";

  Table ht = create_table();

  // Bunch of tests
  insert_to_table(ht, test_keys[0], 1);
  insert_to_table(ht, test_keys[0], 4);
  insert_to_table(ht, test_keys[1], 5);
  insert_to_table(ht, test_keys[1], 2);

  h = poly_hash(test_keys[0]);
  print_list(ht->buckets[h]);
  h = poly_hash(test_keys[1]);
  print_list(ht->buckets[h]);

  test_search(ht, 6, test_keys[0], 1);
  test_search(ht, 6, test_keys[0], 4);
  test_search(ht, 6, test_keys[1], 5);

  delete_in_table(ht, test_keys[1], 5);
  delete_in_table(ht, test_keys[0], 4);
  delete_in_table(ht, test_keys[0], 2);

  h = poly_hash(test_keys[0]);
  print_list(ht->buckets[h]);
  h = poly_hash(test_keys[1]);
  print_list(ht->buckets[h]);

  test_search(ht, 6, test_keys[0], 1);
  test_search(ht, 6, test_keys[0], 4);
  test_search(ht, 6, test_keys[1], 5);
  return 0;
}

