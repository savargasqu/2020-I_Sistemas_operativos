CC=clang # Para BSD
#CC=gcc # Para GNU/LINUX
CFLAGS=-I.
SRC_SERV=src_server
SRC_CLI=src_client
DEPS=p2-shared.h

make: p2-dogServer p2-dogClient


p2-dogServer: src/p2-dogServer.c src/p2-shared.c src/file_operations.c src/table_generator.c
	$(CC) -o $@ -I include/

p2-dogClient: src/p2-dogClient.c src/p2-shared.c
	$(CC) -o $@ -I include/

