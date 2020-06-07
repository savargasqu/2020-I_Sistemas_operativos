#include <stdio.h>
#include <stdlib.h>

#include <ctype.h> // tolower(char)
#include <stdbool.h>
#include <string.h> // strcpy, strlen
#include <time.h>   // randomization
// Multithreading
#include <pthread.h>
// Networking libraries
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// TODO: Write log function signatures

// TODO fix this
// NETWORKING (SHARED)
#define MAX 256
#define PORT "3535"

// FOR SERVER
#define DATA_FILE "dataDogs.dat"
#define LOG_FILE "serverDogs.log"
#define NUM_RECORDS 117

// FOR CLIENT
#define INPUT_WARNING "Ingrese un opción válida\n"

typedef struct {
  char name[32];    // Nombre
  char species[32]; // Tipo
  int age;          // Edad (años)
  char breed[16];   // Raza
  int height;       // Estatura (cm)
  float weight;     // Peso (Kg)
  char sex;         // Sexo (H/M)
  bool deleted;     // If a record is deleted, but the table hasn't been resized
} dogType;

typedef struct {
  FILE *fptr;        // File pointer
  unsigned size;     // Current number of records stored
  unsigned capacity; // Number of records that _can_ be stored
} table_t;

/* MODULE:
 * SHARED FUNCTIONS */
dogType *allocate_record(); // Wrapper
void handle_error(char *);  // Generic error handling.
void send_record(int fd, dogType *);
void recv_record(int fd, dogType *);
void send_id(int fd, unsigned);
unsigned recv_id(int fd);
void send_name(int fd);
char *recv_name(int fd);

// Send/receive a file through a socket
void relay_file_contents(int inputfd, int outputfd);

/* MODULE:
 * SERVER SIDE NETWORK FUNCTIONS */
void srv_menu(table_t *, int fd);
bool srv_insert(table_t *, int fd);
bool srv_view(table_t *, int fd);
void srv_delete(table_t *, int fd);
// TODO: Refactor search
void srv_search(table_t *, int fd);
void srv_confirm_op(int fd);

/* MODULE:
 * SERVER SIDE AUXILIARY FUNCTIONS */
void send_medical_hist(int fd, unsigned id); // => open_medical_hist

/* MODULE:
 * SERVER SIDE LOGIC FUNCTIONS */
unsigned poly_hash(char *s);
bool insert_record(table_t *, dogType *, unsigned);
unsigned probe_table(table_t *, unsigned);
bool view_record(table_t *, dogType *, unsigned);
void delete_record(table_t *, dogType *, unsigned);
// TODO: reestructure search

/* MODULE:
 * CLIENT SIDE */
void cli_menu(int fd);
void cli_insert(int fd);
void cli_view(int fd);
void cli_delete(int fd);
void cli_search(int fd);
void cli_confirm_op(int fd); // => srv_confirm_op

/* MODULE:
 * CLIENT SIDE AUXILIARY AND I/O FUNCTIONS */
bool ask_open_hist();
void open_medical_hist(int fd, unsigned id); // => send_medical_hist

unsigned ask_id(unsigned table_size);
char *ask_name();
void ask_new_record(dogType *);
void str_upper_case(char *str);
void print_record(dogType *, unsigned id);

/* MODULE:
 * FILE OPERATIONS */
table_t *open_table_file();
void close_table_file(table_t *table);
void read_from_table(table_t *table_ptr, dogType *record_ptr);
void write_to_table(table_t *table_ptr, dogType *record_ptr);
void lookup_in_table(table_t *table_ptr, unsigned position);

/* MODULE:
 * RANDOM STRUCTS GENERATOR */
void generate_random_table(table_t *table_ptr);
void generate_random_record(dogType *record);
int generate_random_int(int min, int max);
float generate_random_float(float max);
char *generate_random_string(int str_len);
char generate_random_gender();
