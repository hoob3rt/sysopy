CC = gcc -Wall -std=c11 -ggdb
LIB_O = library.o
LIB__STATIC = library.a
LIB_SO = library.so

create:
	$(CC) -c -fPIC -o $(LIB_O) library.c

static: lib.c
	make create
	ar rcs $(LIB_SO) $(LIB_O)

shared:
	make create
	$(CC) -o $(LIB_SO) -fPIC -shared $(LIB_O)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
	rm -f lib
