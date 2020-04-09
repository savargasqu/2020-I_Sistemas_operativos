#include <stdio.h>
#include <stdlib.h>

// K&R 6.5: Self-referential structures. MUY útil

/* DONE:
 * Linked list (for integers)
 * Basic functions: Insert, search, print.
 *
 * TODO:
 * Doubly linked list
 * - Note that the running time for deletion in a doubly linked list is faster.
 * Comparison functions, for numbers and strings (necessary for search)
 * Hash table of n-buckets with a generic hash function
 * Specific hash functions for numbers and strings
 */

struct node;
struct list;
typedef struct list * List;
typedef struct node * Node;
List create_list();
void insert_to_list(List, int);
//void delete_in_list(List, int);
Node search_list(List, int);
void print_list(List);

struct node {
  int val;   // value stored in the node
  //Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
};

struct list {
  unsigned int len;  // Length of the list (not necessary, but useful)
  Node head; // Pointer to first node on the list
};

List create_list() {
  List list = (List) malloc(sizeof(List));
  struct list newList;
  list->len = 0;
  list->head = NULL;
  return list;
}

void insert_to_list(List list, int val) {
  //if((node_p = search_list(list, val)) == null); //For hash table
  Node node = (Node) malloc(sizeof(Node));
  node->val = val;
  //if(list->head != NULL) {
  //  list->head->prev = node;
  //}
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

//void delete_in_list(List list, int val) {
//  Node node = search_list(list, val);
//  if (node != NULL) {
//    if (node->prev != NULL) { 
//      node->prev->next = node->next; // Change the pointer of prev
//      node->next->prev = node->prev; // Change the pointer of next
//    } else {
//      node->next->prev = NULL; // First node doesn't have a previous node
//    }
//    free(node);
//  }
//}

void print_list(List list) {
  printf("list: ");
  for (Node temp = list->head; temp != NULL; temp = temp->next) {
    printf("%d ", temp->val);
  }
  printf("\n");
}

void test_search(List list, int len_values, int *values) {
  for (int i = 0; i < len_values; i++) {
    Node test = search_list(list, values[i]);
    if (test != NULL) {
      printf("Found %d in list\n", test->val);
    } else {
      printf("didn't found any matching node for %d\n", values[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  List list;
  list = create_list();
  printf("Length of list: %u\n", list->len);

  int val = 1;
  insert_to_list(list, val);

  val = 10;
  insert_to_list(list, val);

  val = 100;
  insert_to_list(list, val);

  printf("Length of list: %u\n", list->len);
  print_list(list);
  const int n = 6;
  int vals[n] = {0, 1, 2, 100, 5, 10};
  test_search(list, n, vals);

  return 0;
}

