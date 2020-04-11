/* Hash Table Implementation
 *
 * Juan Pablo Gutiérrez
 * Tatiana Roberto
 * Sergio Alejandro Vargas
 *
 * Universidad Nacional de Colombia
 * 2020-04-13
 *
 * References:
 * K&R 6.5. Self-referential structures
 * CLRS 11. Hash tables
 */
#include <stdio.h>
#include <stdlib.h>

#define HASHSIZE 11 // Should be a prime number!

// Structures with their correspoding pointer type
struct node;
typedef struct node *Node;
struct list;
typedef struct list *List;
struct table;
typedef struct table *Table;

/* create_table: Allocate memory for a hash table. Initialize buckets as NULL*/
Table create_table();

/* poly_hash: Calculates h(k), the polynomial hash of string k */
long poly_hash(char *);

/* insert_to_table: Creates a node (k, v)and places it in the h(k)-th bucket */
void insert_to_table(Table, char *, int);

/* delete_in_table: Searches for a node. If it finds a match, it removes its
 * reference from the table and frees its memory */
void delete_in_table(Table, char *, int);

/* search_in_table: For a node (k, v), it goes to bucket h(k) an looks for v
 * If it finds it returns a pointer to its node, otherwise it returns NULL */
Node search_in_table(Table, char *, int);
void print_table(Table);

struct node {
  char *key; // Key for hash function
  int value; // value stored in the node
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
};

struct table {
  Node buckets[HASHSIZE];
};


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

void delete_in_list(Table table, char *key, int val) {
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

  delete_in_list(ht, test_keys[1], 5);
  delete_in_list(ht, test_keys[0], 4);
  delete_in_list(ht, test_keys[0], 2);

  h = poly_hash(test_keys[0]);
  print_list(ht->buckets[h]);
  h = poly_hash(test_keys[1]);
  print_list(ht->buckets[h]);

  test_search(ht, 6, test_keys[0], 1);
  test_search(ht, 6, test_keys[0], 4);
  test_search(ht, 6, test_keys[1], 5);
  return 0;
}

/*
struct list {
  unsigned int len; // Length of the list (not necessary, but useful)
  Node head;        // Pointer to first node on the list
};

List create_list() {
  List list = (List)malloc(sizeof(struct list)); // malloc returns a pointer
  struct list newList;
  list->len = 0;
  list->head = NULL;
  return list;
}

void insert_to_list(List list, int val) {
  // if((node = search_list(list, val)) == null); //For hash table
  Node node = (Node)malloc(sizeof(struct node));
  node->value = val;
  if (list->head != NULL) { // The list is not empty
    list->head->prev = node;
  }
  node->prev = NULL;
  node->next = list->head;
  list->head = node;
  list->len += 1;
}

Node search_in_list(List list, int val) {
  for (Node temp = list->head; temp != NULL; temp = temp->next) {
    if (temp->value == val) {
      return temp; // Found
    }
  }
  return NULL; // Not found
}

void delete_in_list(List list, int val) {
  Node node = search_in_list(list, val);
  if (node != NULL) {         // Check if node was found
    if (node->prev != NULL) { // Check if node has a predecessor
      node->prev->next = node->next;
    } else {
      list->head = node->next;
    }
    if (node->next != NULL) { // Check if node has a successor
      node->next->prev = node->prev;
    }
    list->len -= 1;
    free(node); // Free takes a pointer, not a structure
  }
}
*/
