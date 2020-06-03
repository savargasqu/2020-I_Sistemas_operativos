#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>   // For tolower(char)
#include <stdbool.h> // For Boolean types
#include <string.h>  // For strcpy, strlen
#include <time.h>    // For randomization
// Multithreading
#include <pthread.h>
// Networking libraries
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// TODO:
// MOVE MODULE FUNCTIONS DECLARATIONS TO THEIR RESPECTIVE HEADER FILE

// TODO fix this
// NETWORKING (SHARED)
#define MAX 256
#define PORT "3535"

// ONLY FOR SERVER
#define DATA_FILE "dataDogs.dat"
#define LOG_FILE "serverDogs.log"
#define NUM_RECORDS 117

// ONLY FOR CLIENT
#define INPUT_WARNING "Ingrese un opción válida\n"

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

/**** MODULE: SHARED FUNCTIONS ****/
void handle_error(char *func_name);
void relay_file_contents(int inputfd, int outputfd, char *buffer);
dogType *allocate_record();

void send_record(int fd, dogType *);
void recv_record(int fd, dogType *);


/**** MODULE: FILE OPERATIONS ****/
table_t *open_table_file();
void close_table_file(table_t *table);
void read_from_table(table_t *table_ptr, dogType *record_ptr);
void write_to_table(table_t *table_ptr, dogType *record_ptr);
void lookup_in_table(table_t *table_ptr, unsigned position);

/**** MODULE: RANDOMIZATION ****/
void generate_random_table(table_t *table_ptr);
int generate_random_int(int min, int max);
char *generate_random_string(int str_len);
void generate_random_record(dogType *record);

char generate_random_gender();
float generate_random_float(float max);
