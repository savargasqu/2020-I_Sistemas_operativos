#include <ctype.h> // for tolower(char)
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcpy, strlen
#include <time.h>   // For randomization

#define FILE_NAME "people.dat"
#define NUM_RECORDS 10

// Use this simpler struct for development. Replace with pet struct later
typedef struct {
  char name[8];
  int age;
} record_t;

// New table struct
typedef struct {
  FILE *fptr;            // File pointer
  unsigned int size;     // Current number of records stored
  unsigned int capacity; // Number of records that _can_ be stored
} table_t;

/*** MODULE: OPEN ADDRESSING HASH TABLE FILE ***/
unsigned int poly_hash(char *);
table_t *open_table_file();
void close_table_file(table_t *);
void read_record(table_t *, record_t *);
void lookup_record(table_t *, int);

/**** MODULE: MENU OPERATIONS ****/
int insert_record(table_t *table, record_t *record);
void view_record(table_t *table, record_t *temp, int id);
void delete_record(table_t *table, record_t *temp, int id);
int search_record(table_t *table, int position, char *name);
void ask_new_record(record_t *new_record); // aux to insert
void print_record(record_t *record_ptr);   // aux to view
unsigned ask_id(table_t *table);           // aux to delete and search

/*** MODULE: RANDOMIZATION ***/
void generate_random_table(table_t *table_ptr);
int generate_random_int(int min, int max);
char *generate_random_string(int str_len);
void generate_random_record(record_t *record);

