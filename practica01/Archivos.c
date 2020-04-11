#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char menu;
    int id;
    char nombre [32];
    char tipo [32];
    int edad;
    char raza [16];
    int estatura;
    float peso;
    char sexo;
    
    puts("############### SISTEMA HISTORIAS CLINICAS VETERINARIAS ###############\n");
    
    puts("############# MENU: ############# \n");
    printf("1. INGRESAR REGISTRO \n");
    printf("2. VER REGISTRO \n");  
    printf("3. BORRAR REGISTRO \n");
    printf("4. BUSCAR REGISTRO \n");
    printf("5. SALIR \n\n");
    
    printf("Ingrese el número de opción que desea utilizar: ");
    scanf("%c",&menu);
    
    
    if(menu == '1'){
        system("clear");

	char name[32];
	char fileName[64];

    	printf("Identificador hash: ");
    	scanf("%s",name);

	FILE* fichero;
	sprintf(fileName,"%s.txt",name);

        fichero = fopen( fileName , "w");
        
    puts("\n----------------  APERTURA HISTORIA CLINICA  ----------------\n");
    fprintf(fichero,"%s\n", "\n----------------  HISTORIA CLINICA  ----------------");
    printf("Nombre: ");
    scanf("%s",nombre);
    fprintf(fichero,"%s: %s			", "     Nombre", nombre);
    fprintf(fichero,"%s: %s\n", "ID", name);
    fprintf(fichero,"%s\n", "----------------------------------------------------");
    printf("Tipo: ");
    scanf("%s",tipo);
    fprintf(fichero,"%s: %s\n", "	Tipo", tipo);
    printf("Edad: ");
    scanf("%d",&edad);
    fprintf(fichero,"%s: %d\n", "	Edad", edad);
    printf("Raza: ");
    scanf("%s",raza);
    fprintf(fichero,"%s: %s\n", "	Raza", raza);  
    printf("Estatura (cm): ");
    scanf("%d",&estatura);
    fprintf(fichero,"%s: %d\n", "	Estatura", estatura);
    printf("Peso (kg): ");
    scanf("%f",&peso);
    fprintf(fichero,"%s: %.2f\n", "	Peso", peso);
    printf("Sexo: ");
    scanf("%c",&sexo);
    fprintf(fichero,"%s: %c", "	Sexo", sexo);
    fprintf(fichero,"%s\n", "\n----------------------------------------------------");

    fclose(fichero);
    printf("Proceso completado");
    return 0;
    }
    else if(menu == '2'){
        system("clear");


	char name[32];
	char fileName[64];
        char linea[1024];

    	printf("Identificador hash: ");
    	scanf("%s",name);

	FILE* fichero;
	sprintf(fileName,"%s.txt",name);

        fichero = fopen(fileName, "r");
    
    	if (fichero==NULL) {
    	    fputs ("El archivo no existe",stderr); 
    	    exit (1);
    	}
	 else{
		while(fgets(linea,1024,(FILE*)fichero)){
			printf("%s\n",linea);
		}
	}	 
	 fclose ( fichero);
	 return 0;
    }
    //return 0;
}
