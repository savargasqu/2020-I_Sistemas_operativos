#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

# define WAIT 100
# define FILE_NAME "pi_file" // Archivo para guardar el resultado del proceso hijo

int main() {
  FILE *fp;          // Referencia al archivo
  pid_t cpid;        // id de proceso
  double pos_series; // Suma de la parte positiva de la serie
  double neg_series; // Suma de la parte negativa de la serie
  double result;     // Estimacion de pi
  int wait_time;     // Tiempo de espera del proceso padre al hijo

  cpid = fork(); // Crear proceso hijo

  if (cpid == 0) {
    printf("\nPID proceso hijo: %i\n", cpid);

    // Calcular la parte positiva de la serie
    pos_series = 0.0;
    for (int i = 0; i < 10e8; i += 2) {
      pos_series += 1.0/(2*i + 1);
    }

    // Escribir resultado de la parte positiva en un archivo
    fp = fopen(FILE_NAME, "wb");
    fwrite(&pos_series, sizeof(pos_series), 1, fp);
    fclose(fp);

  } else {
    printf("\nPID proceso padre: %i\n", cpid);

    // Calcular la parte negativa de la serie
    neg_series = 0.0;
    for (int i = 1; i < 10e8; i += 2) {
      neg_series += 1.0/(2*i + 1);
    }

    // Esperar a que exista el archivo creado por el hijo
    wait_time = WAIT;
    while(&free) {
      if( access(FILE_NAME, F_OK) != -1 ) { // access es de la libreria unistd

        // Leer resultado del proceso hijo
        fp = fopen("file.dat" , "rb");
        fread(&pos_series, sizeof(pos_series), 1, fp);
        fclose(fp);

        // Imprimir resultado y salir
        result = 4.0*(pos_series - neg_series);
        printf("%f\n", result);
        break;

      } else {
        wait(&wait_time);
      }
    }
  }
  return 0;
}
