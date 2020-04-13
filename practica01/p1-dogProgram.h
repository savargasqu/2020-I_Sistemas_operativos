#include <stdio.h>
#include <stdlib.h>

// Define error macros in spanish
#define OPEN_ERROR "ERROR: Archivo no se pudo abrir con exito"
#define SERIALIZE_ERROR "ERROR: Archivo no se pudo leer/escribir con exito"
#define CLOSE_ERROR "ERROR: Archivo no se pudo cerrar con exito"
#define MEMORY_ERROR "ERROR: Falla en la alocación de memoria"
#define INPUT_WARNING "Ingrese un opción correcta\n"

// Name of binary file
#define DATA_FILE "dataDogs.dat"

// Hash table cardinality. Should be a prime number!
#define HASHSIZE 10111

// Structures with their correspoding pointer type
struct node;
typedef struct node *Node;
struct list;
typedef struct list *List;
struct table;
typedef struct table *Table;
struct dogType;
// typedef struct dogType Dog;

struct dogType {
  char name[32];    // nombre
  char species[32]; // tipo
  int age;          // edad (años)
  char breed[16];   // raza
  int height;       // estatura (cm)
  float weight;     // peso (Kg)
  char sex;         // sexo (H/M)
};

struct node {
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
  char *key; // Name of dogType. Key for hash function
  unsigned long value; // ID of dogType. 4 bytes for hash. 4 bytes for unique ID
  struct dogType dog;
};

struct table {
  unsigned int size;  // Number of NODES (NOT BUCKETS) in the table
  unsigned int count; // Number to generate ID of each node. It never decreases.
  Node buckets[HASHSIZE];
};

/* main: Loads table and calls display_menu */
int main();

/*** USER INTERACTING FUNCTIONS ***/

/* display_menu: Runs loop showing the operations a user can do */
int display_menu();

/* create_record: Create a dogType struct and call insert_to_table */
void create_record();

/* request_data: takes a dogType reference and writes the data input from user.
 * Is an auxiliary function to create_record */
void request_data(struct dogType *);

/* view_record: it calls either display_dog_data or write_clinical_history */
void view_record();

/* display_dog_data: Print the information of the requested dog record.
 * Is an auxiliary function to view_record. */
void display_dog_data(struct dogType *);

/* edit_clinical_history: Opens the dog's clinical history in a text file.
 * Is an auxiliary function to view_record. */
void edit_clinical_history(unsigned long);

/* delete_record:  Calls hash table delete function to remove a dog's record */
void delete_record();

/* search_record: Calls a hash table search to retrieve a dogType's data */
void search_record();

/* request_id:  Prints the number of dog records stored in the table,
 * And asks the user for an ID. returns ID without checking existance */
unsigned long request_id();

/* search_with_id: Searches for a node with a given ID. If found, returns the
 * node */
Node search_with_id(unsigned long id);

/*** HASH TABLE IMPLEMENTATION ***/

/* create_table: Allocate memory for a hash table. Initialize buckets as NULL*/
Table create_table();

/* poly_hash: Calculates the polynomial hash of string S
 * h(S) := \sum_{i = 0}^{|S| - 1} S[i]*31^i \mod |H| */
unsigned int poly_hash(char *);

/* insert_to_table: Creates a node (k, v) and places it in the h(k)-th bucket
 * Returns the value v of the new node */
int insert_to_table(Table, char *, struct dogType);

/* search_value_in_table: Search a node with value val. Values are unique.
 * If a node is found, return a pointer to the node, otherwise return NULL */
Node search_value_in_table(Table table, int val);

/* search_keys_in_table: Takes a key and prints all nodes with that key,
 * return the number of nodes found */
unsigned int search_keys_in_table(Table table, char *key);

/* delete_in_table: Searches for a node. If it finds a match, it removes its
 * reference from the table and frees its memory */
void delete_in_table(Table, Node);

/*** SERIALIZATION ***/

/* serialize: Reads or writes the hash table in a binary file */
Table serialize(Table, char);
