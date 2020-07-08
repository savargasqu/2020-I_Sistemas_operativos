# TODO: Corregir Makefile (OBJDIR)
CC=gcc
CFLAGS=-I$(IDIR)
CFLAGS=-I.
SRCDIR=src
BINDIR=bin


make: bin_files pipes shared_memory client server

bin_files:
	$(CC) src/bin_files.c -o bin/bin_files

pipes:
	$(CC) src/pipes.c -o bin/pipes

shared_memory:
	$(CC) src/shared_memory.c -o bin/shared_memory

client:
	$(CC) src/socket_client.c -o bin/client

server:
	$(CC) src/socket_server.c -o bin/server
