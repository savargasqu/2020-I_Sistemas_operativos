#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
const char gender[] = "HM";


int intN(int n) { return rand() % n; }


char *randomString(int len) {
  char *rstr = malloc((len + 1) * sizeof(char));
  int i;
  for (i = 0; i < len; i++) {
    rstr[i] = alphabet[intN(strlen(alphabet))];
  }
  rstr[len] = '\0';
  return rstr;
}

char *randomGender(int len) {
  char *rstr = malloc((len + 1) * sizeof(char));
  int i;
  for (i = 0; i < len; i++) {
    rstr[i] = gender[intN(strlen(gender))];
  }
  rstr[len] = '\0';
  return rstr;
}

int aleatorio_en_rango(int minimo, int maximo){
   return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

float aleatorio_en_rango_decimal(float minimo, float maximo){
   return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

int main(int argc, char **argv) {
  int count=0;
  int numero=0;
  int attribute;
  char nombre [32];
  char tipo [32];
  int edad;
  char raza [16];
  int estatura;
  float peso;
  char sexo[1];

  // the seed for a new sequence of pseudo-random integers
  // to be returned by rand()
  srand(time(NULL));
  
	printf("MENU\n"); 
	printf("1.Nombres\n"); 
	printf("2.Tipo\n"); 
	printf("3.Edad\n"); 
	printf("4.Raza\n");
	printf("5.Estatura\n");
	printf("6.Peso\n");
	printf("7.Sexo\n\n"); 
	printf("¿Que archivo aleatorio desea generar?:");
	scanf("%i",&attribute);
	printf("\n");

	FILE* fichero;
        

	switch(attribute) {
		case 1 :
			printf("El archivo 'Nombres.txt' ha sidos generado.\n");
			FILE *nombres = fopen("Nombres.txt", "w"); 
  			while(count<1000000){
  			 	numero=aleatorio_en_rango(1, 32);
				strcpy(nombre,randomString(numero));
				fprintf(nombres,"%s\n", nombre);
				//printf("%s\n", nombre);
  				count++;
			
			}
			fclose(nombres);
			break;
		case 2 :
			printf("El archivo 'Tipos' ha sido generado.\n");
			FILE *tipos = fopen("Tipos.txt", "w"); 
  			while(count<1000000){
  			 	numero=aleatorio_en_rango(1, 32);
				strcpy(tipo,randomString(numero));
				fprintf(tipos,"%s\n", tipo);
  				//printf("%s\n", tipo);
  				count++;
			}
			fclose(tipos);
			break;
		case 3 :
			printf( "El archivo 'Edades.txt' ha sido generado.\n");
			FILE *edades = fopen("Edades.txt", "w"); 
  			while(count<1000000){
  			 	edad=aleatorio_en_rango(0, 100);
				fprintf(edades,"%d\n", edad);
  				//printf("%i\n",edad);
  				count++;
			}
			fclose(edades);
			break;
		case 4 :
			printf( "El archivo 'Razas.txt' ha sido generado.\n");
			FILE *razas= fopen("Razas.txt", "w");
  			while(count<1000000){
  			 	numero=aleatorio_en_rango(1, 16);
				strcpy(raza,randomString(numero));
				fprintf(razas,"%s\n", raza);
  				//printf("%s\n", raza);
  				count++;
			}
			fclose(razas);
			break;
			
			break;
		case 5 :
			printf( "El archivo 'Estaturas.txt' ha sido generado.\n");
			FILE *estaturas= fopen("Estaturas.txt", "w");
  			while(count<1000000){
  			 	estatura=aleatorio_en_rango(10, 100);
				fprintf(estaturas,"%d\n", estatura);
  				//printf("%i\n",estatura);
  				count++;
			}
			fclose(estaturas);
			break;
		case 6 :
			printf( "El archivo 'Pesos.txt' ha sido generador.\n");
			FILE *pesos= fopen("Pesos.txt", "w");
  			while(count<1000000){
  			 	peso=aleatorio_en_rango_decimal(0, 100);
  				fprintf(pesos,"%f\n", peso);
  				//printf("%f\n",peso);
  				count++;
			}
			fclose(pesos);
			break;
		case 7 :
			printf( "El archivo 'Sexo.txt' ha sido generado.\n");
			FILE *sexos= fopen("Sexo.txt", "w");
  			while(count<1000000){
				strcpy(sexo,randomGender(1));
				fprintf(sexos,"%s\n", sexo);
  				//printf("%s\n", sexo);
  				count++;
			}
			fclose(sexos);
			break;
		default :
			printf("La opción ingresada no existe\n");
       }
       return 0; 

}
