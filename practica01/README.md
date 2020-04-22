# Práctica 1

**Sistemas Operativos (Grupo 1) 2020-I**

**Universidad Nacional de Colombia**

Juan Pablo Gutiérrez
`jgutierrezre@unal.edu.co`

Tatiana Roberto Torres
`etrobertot@unal.edu.co`

Sergio Alejandro Vargas
`savargasqu@unal.edu.co`


Implementación de:
[os-20201/practica1.pdf](https://github.com/capedrazab/os-20201/blob/master/practica1.pdf)


### Estructura del código:

```
practica01/
|
|-src/
|   |-p1-dogProgram.c: Main
|   |
|   |-hash_table.c: Implementación de la tabla hash
|   |
|   |-tui_menu.c: Menú y demás funciones que interactuan con el usuario
|   |
|   |-serializer.c: Manejo de archivo binario
|   |
|   |-struct_generator.c: Generador de estructuras aleatoreas
|
|-Makefile
|
|-p1-dogProgram.h    Header de todo el proyecto
```

### Uso

Para generar un nuevo archivo binario:
```
$ make new
$ ./struct_generator
```

Para compilar el código y usar la interfaz de texto con un archivo binario ya creado:
```
$ make
$ ./p1-dogProgram
```

### Referencias:

- Kernighan, Ritchie. (1988) _The C Programming Language_ (2da Ed.) § 6.5. Self-referential structures
- Cormen,  Leiserson, Rivest, Stein. (2009) _Introduction to Algorithms_ (3ra Ed.) § 11 Hash Tables
