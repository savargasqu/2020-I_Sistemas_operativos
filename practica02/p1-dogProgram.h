#include <ctype.h> // for tolower(char)
#include <pthread.h> // Multiple threads for different clients
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcpy, strlen
#include <time.h>   // For randomization

#define ERR_OPEN "ERROR en fopen. El archivo no se pudo abrir con éxito"
#define ERR_SEEK "ERROR en fseek. El flujo del archivo no se ajustó con éxito"
#define ERR_READ "ERROR en fread. El archivo no se pudo leer con éxito"
#define ERR_WRITE "ERROR en fwrite. El archivo no se pudo escribir con éxito"
#define ERR_CLOSE "ERROR en fclose. Archivo no se pudo cerrar con éxito"
#define ERR_MEMORY "ERROR en malloc. Falla en la alocación de memoria"

#define FILE_NAME "dataDogs.dat"
#define NUM_RECORDS 117

typedef struct {
  char name[32];    // nombre
  char species[32]; // tipo
  int age;          // edad (años)
  char breed[16];   // raza
  int height;       // estatura (cm)
  float weight;     // peso (Kg)
  char sex;         // sexo (H/M)
  bool deleted;     // If a record is deleted, but the table hasn't been resized
} dogType;

typedef struct {
  FILE *fptr;        // File pointer
  unsigned size;     // Current number of records stored
  unsigned capacity; // Number of records that _can_ be stored
} table_t;

/**** MODULE: FILE OPERATIONS ****/
dogType *allocate_record();
table_t *open_table_file();
void close_table_file(table_t *table);
void read_from_table(table_t *table_ptr, dogType *record_ptr);
void write_to_table(table_t *table_ptr, dogType *record_ptr);
void lookup_in_table(table_t *table_ptr, unsigned position);

/**** MODULE: MENU OPERATIONS ****/
void display_menu(table_t *);

unsigned poly_hash(char *s);

int insert_record(table_t *table, dogType *record, unsigned);
unsigned probe_table(table_t *, unsigned start_pos); // aux to insert
void ask_new_record(dogType *new_record);            // aux to insert

void view_record(table_t *table, dogType *temp, unsigned);
void print_record(dogType *record_ptr, unsigned id); // aux to view
void open_clinical_history(unsigned id);

void delete_record(table_t *table, dogType *temp, unsigned);

void search_record_name(table_t *table, char *name, unsigned);
char *ask_name(); // aux to search_record

unsigned ask_id(table_t *table); // aux to delete and view
void str_lower_case(char *str);  // aux to insert and ask_name/search

/**** MODULE: RANDOMIZATION ****/
void generate_random_table(table_t *table_ptr);
int generate_random_int(int min, int max);
char *generate_random_string(int str_len);
void generate_random_record(dogType *record);

char generate_random_gender();
float generate_random_float(float max);
