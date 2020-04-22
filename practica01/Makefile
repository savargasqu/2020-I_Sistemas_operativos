#CC=clang # Para BSD
CC=gcc # Para GNU/LINUX
CFLAGS=-I.
SRCDIR=src
DEPS=hash_table.h

p1-dogProgram: src/hash_table.c src/serializer.c src/p1-dogProgram.c
	$(CC) -o $@ $^ $(CFLAGS)

struct_generator: src/hash_table.c src/serializer.c src/struct_generator.c
	$(CC) -o $@ $^ $(CFLAGS)

