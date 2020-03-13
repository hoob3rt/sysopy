.DEFAULT_GOAL := all

CC = gcc -Wall -std=c11 -ggdb
SRC = test_suite.c
OUT_STATIC = static
OUT_SHARED = shared
OUT_DYNAMIC = dynamic

LIB_C = ../zad1/library.c
LIB_A = library.a
LIB_O = library.o
LIB_SO = libsysopy.so

FILES = ../test_files/
REDIRECT = >> raport3a.txt

all: static shared clean


compile_static:
	$(CC) -c $(LIB_C)
	ar rcs $(LIB_A) $(LIB_O)

compile_shared:
	$(CC) -c -fPIC $(LIB_C)
	gcc -shared -fPIC $(LIB_O) -o $(LIB_SO)

static: compile_static
	$(CC) -o $(OUT_STATIC) $(SRC) $(LIB_O) 

shared: compile_shared
	$(CC)  -L. -I. $(SRC) -o $(OUT_SHARED) -lsysopy -Wl,-rpath=. 

dynamic:
	$(CC) -o $(OUT_DYNAMIC) $(SRC) -ldl -D DYNAMIC

clean_raport:
	echo "" > raport3a.txt

test_static: static
	echo "--------------------STATIC LIBRARY--------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_shared: shared
	#small comparison test
	echo "--------------------SHARED LIBRARY--------------------" $(REDIRECT)
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2  $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_dynamic: dynamic
	#small comparison test
	echo "--------------------DYNAMIC LIBRARY--------------------" $(REDIRECT)
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test: clean_raport test_static test_shared test_dynamic clean

clean:
	rm -f *.o
	rm -f *.a
	rm -f tmp

clean_all: clean
	rm -f *.so
	rm -f shared
	rm -f dynamic
	rm -f static
