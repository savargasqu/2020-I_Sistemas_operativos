#include "p2-shared.h"

/**** MODULE: CLIENT SIDE ****/
void cli_menu(int fd);
// No insert network fn. Use send_record
void cli_view(int fd, dogType *, int id);
// No delete network fn. Use send_id
void cli_search(int fd, dogType *);

/**** CLIENT SIDE AUXILIARY FUNCTIONS ****/
void cli_confirm_operation(int fd); // => serv_confirm_operation

int request_record(int fd, dogType *); // => serv_send_record
void send_id(int fd, unsigned id);     // => get_id
void send_name(int fd);                // => get_name

void display_medical_hist(int fd, unsigned id); // => send_medical_hist

/**** USER INPUT FUNCTIONS ****/
unsigned ask_id(unsigned table_size);
char *ask_name();
void ask_new_record(dogType *);
void str_lower_case(char *str);
void print_record(dogType *, unsigned id); // Output fn
