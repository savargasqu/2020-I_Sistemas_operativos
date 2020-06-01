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

void recvFile(int clientfd){
    char buff[MAX];  // to store message from client
    char ans; //[Y/n] selection

    FILE *fp;

    fp = fopen("received.txt","w+"); // stores the file content in recieved.txt in the program directory
        if(fp <= 0){
            perror("Error in fopen: ");
            return;
        }

    if(read(clientfd, buff, MAX) > 0){
        fprintf(fp,"%s",buff);
    }
    
    fclose(fp);

    printf("File received successfully.\n");
    printf("Do you wish to open the file? [Y/n]\n");
    while(1){
        scanf(" %c", &ans);
        if(ans == 'Y' || ans == 'y'){
            system("nano ./received.txt");
            break;
        }else if(ans == 'N' || ans == 'n'){
            break;
        }
    }
}

int display_menu(int clientfd) {
    int menu_selection = -1;
    char wait;

    int r;
    dogType buffer;

    while (1) {
        system("clear");
        printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
            "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
            "Ingrese el número de una opción: ");
        scanf("%d", &menu_selection);

        switch (menu_selection) {
        case 1: // Ingresar registro
            send(clientfd,"1",1,0);
            r = recv(clientfd, &buffer, sizeof(dogType), 0);
                if(r < 0){
                    perror("\n-->Error en recv(): ");
                    exit(-1);
                }
            printf("Nombre: %s\nEdad: %u\n", buffer.name, buffer.age);
            break;
        case 2: // Ver registro
            send(clientfd,"2",1,0);
            recvFile(clientfd);
            break;
        case 3: // Borrar registro
            send(clientfd,"3",1,0);
            //delete_record(ht);
            break;
        case 4: // Buscar registro
            send(clientfd,"4",1,0);
            //search_record_by_name(ht);
            break;
        case 5: // Salir
            send(clientfd,"5",1,0);
            return 0;
            break;
        default:
            printf("INPUT WARNING");
            break;
    }

    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait);
  }
}

int main(int argc, char *argv[]){
    int clientfd, r;
    struct addrinfo hints, *res; 

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;// use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;// fill in my IP for me

    getaddrinfo(NULL, PORT, &hints, &res);
    
    clientfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(clientfd < 0){
            perror("\n-->Error in socket():");
            exit(-1);
        }

    r = connect(clientfd, res->ai_addr, res->ai_addrlen);
        if(r < 0){
            perror("\n-->Error in connect(): ");
            exit(-1);
        }else{
            printf("Connected to server...\n");
        }

    display_menu(clientfd);

    close(clientfd);
}