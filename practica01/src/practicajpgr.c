#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct dogType{
   char nombre[32];
   char tipo[32];
   int edad;
   char raza[16];
   int estatura;
   float peso;
   char sexo;
};

int save(void *ap){
   struct dogType *data;
   data = ap;

   fflush(stdin);
   printf("Please enter nombre: ");
   scanf(" %s", data->nombre);
   printf("Please enter tipo:   ");
   scanf(" %s", data->tipo);
   printf("Please enter edad:   ");
   scanf(" %i", &data->edad);
   printf("Please enter raza:   ");
   scanf(" %s", data->raza);
   printf("Please enter estatura:   ");
   scanf(" %i", &data->estatura);
   printf("Please enter peso:   ");
   scanf(" %f", &data->peso);
   printf("Please enter sexo:   ");
   scanf(" %c", &data->sexo);

   FILE *apFile;
   int r;

   apFile = fopen("dataDogs.dat","w");

   if(apFile == NULL){
      perror("cant open file");   
      exit(-1);             
   }

   r = fwrite(ap, sizeof(struct dogType), 1, apFile);
   if(r <= 0){
      perror("cant write file");   
      exit(-1);             
   }

   r = fclose(apFile);
   if(r < 0){
      perror("cant close file");   
      exit(-1);             
   }

}

int load(void *ap){
   struct dogType *data;
   data = ap;
   struct dogType buffer;

   FILE *apFile;
   int r;

   apFile = fopen("dataDogs.dat","r");

   if(apFile == NULL){
      perror("cant open file");   
      exit(-1);             
   }

   fread(&buffer, sizeof(struct dogType), 1, apFile);

   printf(" %s", buffer.nombre);
   printf(" %s", buffer.tipo);
   printf(" %i", buffer.edad);
   printf(" %s", buffer.raza);
   printf(" %i", buffer.estatura);
   printf(" %f", buffer.peso);
   printf(" %c", buffer.sexo);

   r = fclose(apFile);
   if(r < 0){
      perror("cant close file");   
      exit(-1);             
   }

}

int main()
{
   struct dogType *data;
   data = malloc(sizeof(struct dogType));
   if(data == NULL){
      perror("error malloc: ");
      exit(-1);
   }

   bool exit = false;
	char char_choice[3];
	int int_choice = 0;

   do{
      system("cls");
      printf("Main Menu: \n\n");
      printf("1. Ingresar Registro\n");
      printf("2. Ver Registro\n");
      printf("3. Buscar Registro\n");
      printf("4. Borrar Registro\n");
      printf("5. Salir\n");

		scanf("%s", char_choice);
		int_choice = atoi(char_choice);

      switch (int_choice)
      {
         case 1:
            save((void *)data);
            printf("Presione cualquier tecla para continuar.");
            getch();
            break;
         case 2:
            load((void *)data);
            printf("Presione cualquier tecla para continuar.");
            getch();
            // Not yet used.
            break;
         case 3:
            printf("sin implementar.");
            getch();
            // Not yet used.
            break;
         case 4:
            printf("sin implementar.");
            getch();
            // Not yet used.
            break;
         case 5:
            exit = true;
            break;
         default: 
            printf("Ingrese una opcion correcta.");
            getch();
            break;
      }
   }while(!exit);

   free(data);

   return 0;
}
