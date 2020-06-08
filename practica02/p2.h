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

// NETWORKING
#define MAX 256
#define PORT "7878"

// FOR SERVER
#define DATA_FILE "data/dataDogs.dat"
#define LOG_FILE "data/serverDogs.log"
// TODO: Use big numbers
// Total capacity of table (big prime number. load factor of 0.8)
#define NUM_RECORDS 12500227
// Initial size of table
#define GENERATED_RECORDS 10000000

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


typedef struct {
  table_t *table;
  int socketfd;
  char *IP;
} thr_info_t;

/* MODULE:
 * SHARED FUNCTIONS */
void handle_error(char *);  // Generic error handling.
dogType *allocate_record(); // Wrapper
void send_record(int fd, dogType *);
void recv_record(int fd, dogType *);
void send_id(int fd, unsigned);
unsigned recv_id(int fd);
void send_name(int fd, char *name);
char *recv_name(int fd);
// Send/receive a file through a socket
void send_file(int fd, char *file_name);
void recv_file(int fd, char *file_name);
void relay_file_contents(int inputfd, int outputfd);
void *get_in_addr(struct sockaddr *sa); // Get IP

/* MODULE:
 * CLIENT SIDE */
void cli_menu(int fd);
void cli_insert(int fd);
void cli_view(int fd);
void cli_delete(int fd);
void cli_search(int fd);

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
 * SERVER SIDE NETWORK FUNCTIONS */
//void srv_menu(table_t *, int fd, char *ip);
void *srv_menu(void *);
void srv_insert(int fd, table_t *, dogType *, unsigned);
void srv_view(int fd, table_t *, unsigned);
void srv_delete(int fd, table_t *, unsigned);
void srv_search(int fd, table_t *, char *);
void send_medical_hist(int fd, unsigned);

/* MODULE:
 * HASH TABLE LOGIC OPERATIONS */
unsigned poly_hash(char *s);
bool insert_record(table_t *, dogType *, unsigned);
unsigned probe_table(table_t *, unsigned);
bool view_record(table_t *, dogType *, unsigned);
void delete_record(table_t *, dogType *, unsigned);
// Search requires sending many results to the client. it's now a network fn

/* MODULE:
 * FILE OPERATIONS */
table_t *open_table_file(bool);
void close_table_file(table_t *table);
void read_from_table(table_t *table_ptr, dogType *record_ptr);
void write_to_table(table_t *table_ptr, dogType *record_ptr);
void lookup_in_table(table_t *table_ptr, unsigned position);
/* LOG OPERATIONS */
FILE *open_log();
void close_log(FILE *p_log);
void write_to_log(FILE *p_log, char *op, char *name, unsigned id, char *IP);

/* MODULE:
 * RANDOM STRUCTS GENERATOR */
void generate_random_table(table_t *table_ptr);
void generate_random_record(dogType *record);
int generate_random_int(int min, int max);
float generate_random_float(float max);
char *generate_random_string(int str_len);
char generate_random_gender();
