#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  unsigned long ID; // ID of dogType. 4 bytes for hash. 4 bytes for unique ID
};

struct node {
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
  char *key; // Name of dogType. Key for hash function
  struct dogType dog;
};

struct table {
  unsigned long size; // Number of NODES (NOT BUCKETS) in the table
  unsigned long count; // Number to generate IDs. It never decreases.
  Node buckets[HASHSIZE];
};

/* main: Loads table and calls display_menu, once the user sessions ends,
 * it saves the modified table. */
int main();

/******** MENU / USER INTERACTING FUNCTIONS ********/

/* display_menu: Runs loop showing the operations a user can do */
int display_menu(Table);

/* create_record: Create a dogType struct and call insert_to_table */
void create_record(Table);

/* view_record: it either display_dog_data or write_clinical_history */
void view_record(Table);

/* delete_record:  Calls hash table delete function to remove a dog's record */
void delete_record(Table);

/* search_record_by_name: Calls a hash table search to retrieve a dogType's data
 */
void search_record_by_name(Table);

/******** MENU / AUXILIARY FUNCTIONS ********/

/* request_data: takes a dogType reference and writes the data input from user.
 * Is an auxiliary function to create_record */
void request_data(struct dogType *);

/* display_dog_data: Print to stdout the information of the requested dog.
 * Is an auxiliary function to view_record. */
void display_dog_data(struct dogType *);

/* display_clinical_history: Opens the dog's clinical history in a text file.
 * Is an auxiliary function to view_record. */
void display_clinical_history(unsigned long);

/* search_with_id:  Prints the number of dog records stored in the table,
 * And requests an ID from the user, searches for a node with said ID,
 * and returns the node. If no node is found, it returns NULL */
Node search_by_id(Table);

/* string_lower_case: Takes a string and converts it to lower case.
 * The string is overwritten, not copied. */
void string_lower_case(char *);

/******** HASH TABLE IMPLEMENTATION ********
 * NOTE: `value' is the same as the dog ID. One is just a more general name. */

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



/******** SERIALIZATION ********/

void save_table(Table table);
Table load_table();

/* serialize: Reads or writes the hash table in a binary file */
// Table serialize(Table, char);


/******** RANDOM DOG RECORD GENERATOR ********/

int generate_random_int(int, int);
float generate_random_float(float);
char *generate_random_string(int);
char generate_random_gender();
struct dogType generate_dog();
Table generate_table();

