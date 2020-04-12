/* Práctica 1
 * Sistemas Operativos
 * Universidad Nacional de Colombia
 *
 * Juan Pablo Gutiérrez
 * Tatiana Roberto
 * Sergio Alejandro Vargas
 *
 * 2020-04-13
 */
#include <stdio.h>
#include <stdlib.h>

// Define error macros in spanish
#define OPEN_ERROR "ERROR: Archivo no se pudo abrir con exito"
#define WRITE_ERROR "ERROR: Archivo no se pudo escribir con exito"
#define READ_ERROR "ERROR: Archivo no se pudo leer con exito"
#define CLOSE_ERROR "ERROR: Archivo no se pudo cerrar con exito"
#define MEMORY_ERROR "ERROR: Falla en la alocación de memoria"
#define INPUT_WARNING "Ingrese un opción correcta\n"

// Hash table cardinality. Should be a prime number!
#define HASHSIZE 11

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
  char *key; // Key for hash function
  int value; // value stored in the node
  Node prev; // Pointer to the previous node. First node points to NULL
  Node next; // Pointer to the following node. Last node points to NULL
};

struct table {
  Node buckets[HASHSIZE];
};

/******** USER SIDE FUNCTIONS **********/

/* request_data: takes a dogType reference and writes the data input from user.
 * Is an auxiliary function to create_entry */
void request_data(struct dogType *dog_entry);

/* create_entry: Create a dogType struct and writes it to a bin file*/
struct dogType *create_entry(struct dogType *dog_entry);

/* view_entry: Calls either display_dog_data or write_clinical_history */
void view_entry(struct dogType *data);

/* display_dog_data: Print the information of the requested dogType.
 * Is an auxiliary function to view_entry. */
void display_dog_data();

/* edit_clinical_history: Opens the dog's clinical history in a text file.
 * Is an auxiliary function to view_entry. */
void edit_clinical_history();

/* search_entry: Calls the hash table search to retrieve a dogType's data */
void search_entry();

/* delete_entry: Calls the hash table delete to remove a dogType's data */
void delete_entry();

/* menu: Run loop showing the operations a user can do */
int menu();
/*main: Calls menu*/
int main();

/***** HASH TABLE IMPLEMENTATION *******/

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
