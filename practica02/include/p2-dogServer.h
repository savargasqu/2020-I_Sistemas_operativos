#include "p2-shared.h"

/**** MODULE: SERVER SIDE NETWORK FUNCTIONS ****/
void srv_menu(table_t *, int fd);
// No insert network fn. Use get_record
void srv_view(int fd, unsigned id);
// No delete network fn. Use get_id
void srv_search(int fd, table_t *, char *name, unsigned pos);

/**** SERVER SIDE AUXILIARY NETWORK FUNCTIONS ****/
void srv_confirm_operation(int fd); // TODO

void srv_get_record(int fd, dogType *);
int srv_send_record(int fd, table_t *, dogType *); // => request_record

int get_id(int socketfd); // => send_id
char *get_name(int fd);   // => send_name

void send_medical_hist(int fd, unsigned id); // => display_medical_hist

/**** MODULE: SERVER SIDE LOGIC FUNCTIONS ****/
unsigned poly_hash(char *s);
int insert_record(table_t *, dogType *, unsigned pos);
unsigned probe_table(table_t *, unsigned pos);
void delete_record(table_t *, dogType *, unsigned id);
