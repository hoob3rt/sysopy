.DEFAULT_GOAL := all
CC = gcc -Wall -std=c11 -ggdb 
LIBRARY_O = library.o
LIBRARY_STATIC = library.a
LIBRARY_SO = library.so

create:
	$(CC) -c -fPIC -o $(LIBRARY_O) library.c

static: create
	ar rcs $(LIBRARY_STATIC) $(LIBRARY_O)

shared: create
	$(CC) -o $(LIBRARY_SO) -fPIC -shared $(LIBRARY_O)

all: static shared 

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
