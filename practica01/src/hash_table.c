/* Hash Table Implementation
 *
 * Juan Pablo Gutiérrez
 * Tatiana Roberto
 * Sergio Alejandro Vargas
 *
 * Universidad Nacional de Colombia
 * 2020-04-13
 */
#include "../hash_table.h"

Table create_table() {
  Table ht = (Table)malloc(sizeof(struct table));
  for (unsigned long i = 0; i < HASHSIZE; i++) {
    ht->buckets[i] = NULL;
  }
  return ht;
}

long poly_hash(char *k) {
  // Polynomial hash function h(S) = \sum_{i = 0}^{|S| - 1} S[i]*31^i \mod p
  // const unsigned int p = BIGPRIME;
  const unsigned int x = 31;
  unsigned long hash_value = 0;
  for (int i = 0; k[i] != '\0'; i++) {
    hash_value = hash_value * x + (k[i] - 'a' + 1); // % p
  }
  return hash_value % HASHSIZE; // Hash can't exceed the size of the table
}

void insert_to_table(Table table, char *key, int val) {
  // Create a node
  Node node = (Node)malloc(sizeof(struct node));
  node->key = key;
  node->value = val;
  // Compute hash
  long hash = poly_hash(key);
  Node head = table->buckets[hash];
  // Insert node at the beginning of the list
  if (head != NULL) { // The list is not empty
    head->prev = node;
  }
  node->prev = NULL;
  node->next = head;
  table->buckets[hash] = node; // Rebase head
}

Node search_in_table(Table table, char *key, int val) {
  long hash = poly_hash(key);
  for (Node temp = table->buckets[hash]; temp != NULL; temp = temp->next) {
    if (temp->value == val) {
      return temp; // Found
    }
  }
  return NULL; // Not found
}

void delete_in_table(Table table, char *key, int val) {
  Node node = search_in_table(table, key, val);
  if (node != NULL) {         // Check if node was found
    if (node->prev != NULL) { // Check if node has a predecessor
      node->prev->next = node->next;
    } else {
      //list->head = node->next;
      table->buckets[poly_hash(key)] = node->next;
    }
    if (node->next != NULL) { // Check if node has a successor
      node->next->prev = node->prev;
    }
    free(node); // Free takes a pointer, not a structure
  }
}

