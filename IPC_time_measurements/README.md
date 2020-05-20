Sergio Alejandro Vargas

Sistemas Operativos

2020-05-20

# Medición de tiempos para diferentes IPC

Hacer una comparación de los diferentes metodos de comunicación entre procesos (IPC)
y graficar los resultados.

1. Implementar los siguientes IPC entre dos procesos:
- Archivos
- memoria compartida
- _pipes_
- _sockets_
2. Probar cada IPC pasando datos de tamaño:
- `  1 kB`
- ` 10 kB`
- `100 kB`
- `  1 MB`
- ` 10 MB`
- `100 MB`

Anotaciones:

- cuando el proceso consumidor recibe todos los datos, notifica y termina.
- Medir el tiempo de ejecución con `time ./program`.
- las gráficas deben ser logarítmicas en el eje $X$.

