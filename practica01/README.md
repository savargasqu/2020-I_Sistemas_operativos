Juan Pablo Gutiérrez
`jgutierrezre@unal.edu.co`

Tatiana Roberto Torres
`etrobertot@unal.edu.co`

Sergio Alejandro Vargas
`savargasqu@unal.edu.co`

**Sistemas Operativos 2016707-1\
2020-I\
Universidad Nacional de Colombia**

# Práctica 1

Implementación de:
[os-20201/practica1.pdf](https://github.com/capedrazab/os-20201/blob/master/practica1.pdf)

### Estructura del código:

```
practica01/
  |
  |-src/
  |  |
  |  |- file_operations.c: Llamadas al sistema, con sus respectivo manejo de errores
  |  |
  |  |- p1-dogProgram.c: Main, Menú y funciones que interactuan con el usuario
  |  |
  |  |- table_generator.c: Generador de tablas aleatoreas.
  |
  |- Makefile
  |
  |- p1-dogProgram.h    Header de todo el proyecto
```

### Uso

Para compilar el código y usar la interfaz de texto con un archivo binario ya creado:
```
$ make
$ ./p1-dogProgram
```

### Referencias:

- Kernighan, Ritchie. (1988) _The C Programming Language_ (2da Ed.) § 6.5. Self-referential structures
- Cormen,  Leiserson, Rivest, Stein. (2009) _Introduction to Algorithms_ (3ra Ed.) § 11. Hash Tables
