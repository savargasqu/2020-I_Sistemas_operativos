CC=clang
#CC=gcc # Para GNU/LINUX
CFLAGS=-I.
DEPS=p2.h

make: p2-dogClient p2-dogServer table_generator

p2-dogClient: obj/p2-dogClient.o obj/shared_fn.o
	$(CC) $^ -o $@

p2-dogServer: obj/p2-dogServer.o obj/shared_fn.o obj/table_ops.o obj/file_ops.o
	$(CC) $^ -o $@

table_generator: obj/table_generator.o obj/shared_fn.o obj/table_ops.o obj/file_ops.o
	$(CC) $^ -o $@

obj/p2-dogClient.o: src/p2-dogClient.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/p2-dogServer.o: src/p2-dogServer.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/table_generator.o: src/table_generator.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/shared_fn.o: src/shared_fn.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/table_ops.o: src/table_ops.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/file_ops.o: src/file_ops.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm obj/*.o
