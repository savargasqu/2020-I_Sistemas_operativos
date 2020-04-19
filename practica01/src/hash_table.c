#include "../p1-dogProgram.h" // Only for ccls. Make should handle dependencies

Table create_table() {
  Table table = (Table)malloc(sizeof(struct table));
  for (unsigned long i = 0; i < HASHSIZE; i++) {
    table->buckets[i] = NULL;
  }
  table->size = 0;
  table->count = 0;
  return table;
}

unsigned int poly_hash(char *s) {
  const unsigned int x = 31;
  unsigned int hash_value = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    hash_value = hash_value * x + (s[i] - 'a' + 1); // % p
  }
  return hash_value % HASHSIZE; // Hash can't exceed the size of the table
}

int insert_to_table(Table table, char *key, struct dogType dog) {
  Node node, head;
  unsigned int hash;
  unsigned long value;
  // Compute hash
  hash = poly_hash(key);

  // Dog ID is only created if it doesn't have any. For persistence
  if (dog.ID <= 0) {
    dog.ID = (hash << 4) + table->count; // Assign unique ID
  }

  // Create a node
  node = (Node)malloc(sizeof(struct node));
  if (node == NULL) {
    perror(MEMORY_ERROR);
    return -1;
  }
  node->key = key;
  node->dog = dog;
  // node->value = value; // Assign unique ID
  // Insert node at the beginning of the list
  head = table->buckets[hash];
  if (head != NULL) { // The list is not empty
    head->prev = node;
  }
  node->prev = NULL;
  node->next = head;
  table->buckets[hash] = node; // Rebase head
  table->size += 1;
  table->count += 1;

  return dog.ID;
}

Node search_value_in_table(Table table, int val) {
  unsigned int hash = (val >> 4); // Recover hash from value
  for (Node temp = table->buckets[hash]; temp != NULL; temp = temp->next) {
    if (temp->dog.ID == val) {
      return temp; // Found
    }
  }
  return NULL; // Not found
}

unsigned int search_keys_in_table(Table table, char *key) {
  unsigned int hash = poly_hash(key);
  unsigned int count = 0;
  for (Node temp = table->buckets[hash]; temp != NULL; temp = temp->next) {
    if (strcmp(temp->key, key) == 0) {
      printf("%lu: %s\n", temp->dog.ID, temp->key);
      count += 1;
    }
  }
  return count; // Not found
}

void delete_in_table(Table table, Node node) {
  if (node != NULL) {         // Check if node was found
    if (node->prev != NULL) { // Check if node has a predecessor
      node->prev->next = node->next;
    } else {
      // list->head = node->next;
      unsigned int hash = (node->dog.ID >> 4); // Recover hash from value
      table->buckets[(node->dog.ID >> 4)] = node->next;
    }
    if (node->next != NULL) { // Check if node has a successor
      node->next->prev = node->prev;
    }
    table->size -= 1;
    free(node); // Free takes a pointer, not a structure
  }
}
