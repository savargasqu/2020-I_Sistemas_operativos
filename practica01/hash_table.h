/* Hash Table Implementation
 *
 * Juan Pablo Gutiérrez
 * Tatiana Roberto
 * Sergio Alejandro Vargas
 *
 * Universidad Nacional de Colombia
 * 2020-04-13
 */
#include <stdio.h>
#include <stdlib.h>

// Structures with their correspoding pointer type
struct node;
typedef struct node *Node;
struct list;
typedef struct list *List;
struct table;
typedef struct table *Table;

#define HASHSIZE 11 // Should be a prime number!

struct node {
  char *key; // Key for hash function
  int value; // value stored in the node
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
};

struct table {
  Node buckets[HASHSIZE];
};

/* create_table: Allocate memory for a hash table. Initialize buckets as NULL*/
Table create_table();

/* poly_hash: Calculates h(k), the polynomial hash of string k */
long poly_hash(char *);

/* insert_to_table: Creates a node (k, v)and places it in the h(k)-th bucket */
void insert_to_table(Table, char *, int);

/* search_in_table: For a node (k, v), it goes to bucket h(k) an looks for v
 * If it finds it returns a pointer to its node, otherwise it returns NULL */
Node search_in_table(Table, char *, int);

/* delete_in_table: Searches for a node. If it finds a match, it removes its
 * reference from the table and frees its memory */
void delete_in_table(Table, char *, int);

