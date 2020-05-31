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
  bool deleted; // If a record is deleted, but the table hasn't been resized
} record_t;

typedef struct {
  FILE *fptr;        // File pointer
  unsigned size;     // Current number of records stored
  unsigned capacity; // Number of records that _can_ be stored
} table_t;

/**** MODULE: FILE OPERATIONS ****/
record_t *allocate_record();
table_t *open_table_file();
void close_table_file(table_t *table);
void read_from_table(table_t *table_ptr, record_t *record_ptr);
void write_to_table(table_t *table_ptr, record_t *record_ptr);
void lookup_in_table(table_t *table_ptr, unsigned position);

/**** MODULE: MENU OPERATIONS ****/
int display_menu(table_t *, record_t *);

unsigned poly_hash(char *s);

int insert_record(table_t *table, record_t *record, unsigned);
unsigned probe_table(table_t *, unsigned start_pos); // aux to insert
void ask_new_record(record_t *new_record);            // aux to insert

void view_record(table_t *table, record_t *temp, unsigned);
void print_record(record_t *record_ptr); // aux to view

void delete_record(table_t *table, record_t *temp, unsigned);

void search_record_name(table_t *table, char *name, unsigned);
char *ask_name(); // aux to search_record

unsigned ask_id(table_t *table); // aux to delete and view
void str_lower_case(char *str);  // aux to insert and ask_name/search

/**** MODULE: RANDOMIZATION ****/
void generate_random_table(table_t *table_ptr);
int generate_random_int(int min, int max);
char *generate_random_string(int str_len);
void generate_random_record(record_t *record);
