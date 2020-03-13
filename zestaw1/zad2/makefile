.DEFAULT_GOAL := all

CC = gcc -Wall -std=c11 -ggdb
SRC = test_suite.c
OUT = test_suite
LIB_C = ../zad1/library.c
LIB_A = library.a
LIB_O = library.o
FILES = ../test_files/
REDIRECT = >> raport2.txt

all: compile clean

compile_lib:
	$(CC) -c $(LIB_C)
	ar rcs $(LIB_A) $(LIB_O)

compile: compile_lib
	$(CC) -o $(OUT) $(SRC) $(LIB_O)

test: all
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" > raport2.txt
	./$(OUT) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)
	make clean

clean:
	rm -f *.o
	rm -f *.a
	rm -f tmp

clean_all: clean
	rm -f *.so
	rm -f $(OUT)
