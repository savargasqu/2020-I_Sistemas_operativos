#include <stdio.h>
#include <stdlib.h>

/* K&R 6.5: Self-referential structures. MUY útil
 * TODO:
 * Comparison functions, for numbers and strings (necessary for search)
 * Hash table of n-buckets with a generic hash function
 * Specific hash functions for numbers and strings
 */

// Structures with their correspoding pointer type
struct node;
typedef struct node *Node;
struct list;
typedef struct list *List;
// struct table;
// typedef struct table *HashTable;

// List methods
List create_list();
void insert_to_list(List, int);
void delete_in_list(List, int);
Node search_list(List, int);
void print_list(List);

struct node {
  int val;   // value stored in the node
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
};

struct list {
  unsigned int len; // Length of the list (not necessary, but useful)
  Node head;        // Pointer to first node on the list
};

/*
struct table {
  int size; // Cardinality of the hash table
  List *buckets;
};
HashTable create_hash_table(int size) {
  HashTable ht = (HashTable) malloc(sizeof(struct table)); // Malloc retorna un
apuntador ht->size = size; List buckets[size]; for (int i = 0; i < size; i++) {
    buckets[i] = create_list();
  }
  ht->buckets = buckets;
  return ht;
}
*/

List create_list() {
  List list = (List)malloc(sizeof(struct list)); // Malloc retorna un apuntador
  struct list newList;
  list->len = 0;
  list->head = NULL;
  return list;
}

void insert_to_list(List list, int val) {
  // if((node = search_list(list, val)) == null); //For hash table
  Node node = (Node)malloc(sizeof(struct node));
  node->val = val;
  if (list->head != NULL) { // The list is not empty
    list->head->prev = node;
  }
  node->prev = NULL;
  node->next = list->head;
  list->head = node;
  list->len += 1;
}

Node search_list(List list, int val) {
  for (Node temp = list->head; temp != NULL; temp = temp->next) {
    if (temp->val == val) {
      return temp; // Found
    }
  }
  return NULL; // Not found
}

void delete_in_list(List list, int val) {
  Node node = search_list(list, val);
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

void print_list(List list) { // For debugging
  printf("|L| = %u; L = [", list->len);
  if (list->head != NULL) { // Check if list is empty
    Node temp = list->head;
    printf("%d", temp->val);
    while (temp != NULL) {
      printf(", %d", temp->val);
      temp = temp->next;
    }
  }
  printf("]\n");
}

void test_search(List list, int len_values, int *values) { // For debugging
  for (int i = 0; i < len_values; i++) {
    Node test = search_list(list, values[i]);
    if (test != NULL) {
      printf("Found %d\n", test->val);
    } else {
      printf("Didn't found %d\n", values[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  List list;
  const int n = 5;
  int vals[n] = {1, 2, 100, 5, 10};
  int val;

  list = create_list();
  print_list(list);

  insert_to_list(list, 1);
  insert_to_list(list, 10);
  insert_to_list(list, 100);
  print_list(list);

  test_search(list, n, vals);

  delete_in_list(list, 10);
  delete_in_list(list, 100);
  print_list(list);

  test_search(list, n, vals);

  delete_in_list(list, 1);
  print_list(list);
  test_search(list, n, vals);

  return 0;
}

