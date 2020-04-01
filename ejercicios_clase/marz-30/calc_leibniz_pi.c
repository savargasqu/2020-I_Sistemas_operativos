#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define WAIT 100
#define PRES 10e8 // Iteraciones de la serie

int main() {


  FILE *fp;                     // referencia al archivo
  char fname[] = "pi_file.dat"; // Archivo que guarda resultado del proceso hijo
  pid_t cpid;                   // id de proceso
  unsigned long i;
  double pos_series; // Suma de la parte positiva de la serie
  double neg_series; // Suma de la parte negativa de la serie
  double result;     // Estimacion de pi
  int wait_time;     // Tiempo de espera del proceso padre al hijo

  cpid = fork(); // Crear proceso hijo

  if (cpid == 0) {
    printf("PID proceso hijo: %i\n", cpid);

    // Calcular la parte positiva de la serie
    pos_series = 0.0;
    for (i = 0; i < PRES; i += 2) {
      pos_series += 1.0 / (2 * i + 1);
    }

    // Escribir resultado en un archivo
    fp = fopen(fname, "wb");
    fwrite(&pos_series, sizeof(pos_series), 1, fp);
    fclose(fp);

  } else {
    printf("PID proceso padre: %i\n", cpid);

    // Calcular la parte negativa de la serie
    neg_series = 0.0;
    for (i = 1; i < PRES; i += 2) {
      neg_series += 1.0 / (2 * i + 1);
    }

    // Esperar a que exista el archivo creado por el hijo
    wait_time = WAIT;
    waitpid(0, &wait_time, 0); // Waitpid en 0 espera cualquier proceso hijo

    if (access(fname, F_OK) != -1) { // La fun. access es de unistd
      // Leer resultado del proceso hijo
      fp = fopen(fname, "rb");
      fread(&pos_series, sizeof(pos_series), 1, fp);
      fclose(fp);

      // Imprimir resultado y salir
      result = 4.0 * (pos_series - neg_series);
      printf("%.10f\n", result);
    }
  }
  return 0;
}
