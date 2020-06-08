# Práctica 2

Juan Pablo Gutiérrez
`jgutierrezre@unal.edu.co`

Sergio Alejandro Vargas
`savargasqu@unal.edu.co`

**Sistemas Operativos
2016707-1\
2020-I\
Universidad Nacional de Colombia**


Implementación de:
[os-20201/practica2.pdf](https://github.com/capedrazab/os-20201/blob/master/practica2.pdf)


### Estructura del código:

```
practica02/
  |
  |-src/
  |  |
  |  |-file_ops.c        // Manejo de archivos (data y log)
  |  |
  |  |-table_ops.c       // Tabla hash de _open addressing_
  |  |
  |  |-table_generator.c // Registros aleatoreos
  |  |
  |  |-p2-dogClient.c    // interfaz de usuario
  |  |
  |  |-p2-dogServer.c    // Manejo de sockets e hilos
  |  |
  |  |-shared_fn.c       // Funciones comunes entre cliente y servidor (ej. send_id)
  |
  |- Makefile
  |
  |- p2.h // Header de todo el proyecto
  |
```

### Uso

Para compilar los dos ejecutables: `$ make`

Para eliminar los ejecutables y archivos objeto: `$ make clean`

Para eliminar los datos, historias medicas y el log: `$ make clean_data`

