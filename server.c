#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 100
#define PORT "3535"
#define BACKLOG 2

typedef struct dogType{
  char name[32];    // nombre
  char species[32]; // tipo
  int age;          // edad (años)
  char breed[16];   // raza
  int height;       // estatura (cm)
  float weight;     // peso (Kg)
  char sex;         // sexo (H/M)
  bool deleted;     // If a record is deleted, but the table hasn't been resized
} dogType;

dogType test = {.name="Test", .age=77};

void sendFile(int clientfd){ 
    char buff[MAX];

    FILE *fp;
    fp = fopen("sent.txt","r+");
        if(fp == NULL){//If file does not exist, create it.
            printf("File does not exist, creating.\n");
            fp = fopen("sent.txt", "w");
        }
 
    if(fgets(buff, MAX, fp) != NULL){//Fgets reads upto MAX character or EOF.
        write(clientfd, buff, sizeof(buff));//Send the file data to stream.
    }

    fclose(fp);

    printf("File sent successfully.\n");
}

int menulogic(int clientfd) {
    char menu_selection = '0';
    int r;

    while(1){
        r = recv(clientfd, &menu_selection, 1, 0);
            if(r < 0){
                perror("\n-->Error en recv(): ");
                exit(-1);
            }
        printf("user input: %c\n", menu_selection);

        switch(menu_selection) {
        case '1': // Ingresar registro
            r = send(clientfd, &test, sizeof(dogType), 0);//Send data to previously known socket descriptor.
                if(r < 0){//Error-checking.
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }
            break;
        case '2': // Ver registro
            sendFile(clientfd);
            //view_record(ht);
            break;
        case '3': // Borrar registro
            printf("not yet implemented\n");
            //delete_record(ht);
            break;
        case '4': // Buscar registro
            printf("not yet implemented\n");
            //search_record_by_name(ht);
            break;
        case '5': // Salir
            return 0;
            break;
        default:
            printf("INPUT WARNING\n");
            break;
        }
    }
}

int main(){
    struct sockaddr_storage client;
    socklen_t addr_size;
    struct addrinfo hints, *res;

    int serverfd, clientfd, r;

    int yes = 1;//For setsockopt() SO_REUSEADDR, below.
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;//Use IPv4 or IPv6, whichever.
    hints.ai_socktype = SOCK_STREAM;//Use stream sockets.
    hints.ai_flags = AI_PASSIVE;//Fill in my IP for me.

    getaddrinfo(NULL, PORT, &hints, &res);

    serverfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);//Specify the required socket.
        if(serverfd < 0){//Error-checking.
            perror("\n-->Error en socket():");
            exit(-1);
        }

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));//Reuse port.

    r = bind(serverfd, res->ai_addr, res->ai_addrlen);//Associate a socket with a port.
        if(r < 0){//Error-checking.
            perror("\n-->Error en bind(): ");
            exit(-1);
        }

    r = listen(serverfd, BACKLOG);//Listen for incoming connections, queue them.
        if(r < 0){//Error-checking.
            perror("\n-->Error en Listen(): ");
            exit(-1);
        }
    
    printf("server: waiting for connections...\n");

    while(1){//Main accept() loop.
        addr_size = sizeof(client);//Set size to client
        clientfd = accept(serverfd, (struct sockaddr *)&client, &addr_size);//Get pending connection, returns new socket descriptor.
            if(clientfd < 0){//Error-checking.
                perror("\n-->Error en accept: ");
                continue;
            }

        inet_ntop(client.ss_family, (struct sockaddr *)&client, s, sizeof(s));
        printf("server: %s has connected.\n", s);

        menulogic(clientfd);

        printf("server: %s has disconnected.\n", s);

        close(clientfd);
    }
}