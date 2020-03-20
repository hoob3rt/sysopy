.DEFAULT_GOAL := all

CC = gcc -Wall -std=c11 -ggdb
SRC = test_suite.c
OUT_STATIC_O = statico
OUT_STATIC_O3 = statico3
OUT_STATIC_Os = staticos
OUT_SHARED_O = sharedo
OUT_SHARED_O3 = sharedo3
OUT_SHARED_Os = sharedos
OUT_DYNAMIC_O = dynamico
OUT_DYNAMIC_O3 = dynamico3
OUT_DYNAMIC_Os = dynamicos

LIB_C = ../zad1/library.c
LIB_A = library.a
LIB_O = library.o
LIB_SO = libsysopy.so

FILES = ../test_files/
REDIRECT = >> raport3b.txt

all:  static_0 static_03 static_0s shared_O shared_O3 shared_Os dynamic_O dynamic_O3 dynamic_Os clean

compile_static:
	$(CC) -c $(LIB_C)
	ar rcs $(LIB_A) $(LIB_O)

compile_shared:
	$(CC) -c -fPIC $(LIB_C)
	gcc -shared -fPIC $(LIB_O) -o $(LIB_SO)

static_0: compile_static
	$(CC) -o $(OUT_STATIC_O) -O $(SRC) $(LIB_O) 

static_03: compile_static
	$(CC) -o $(OUT_STATIC_O3) -O3 $(SRC) $(LIB_O) 

static_0s: compile_static
	$(CC) -o $(OUT_STATIC_Os) -Os $(SRC) $(LIB_O) 

shared_O: compile_shared
	$(CC)  -L. -I. $(SRC) -O -o $(OUT_SHARED_O) -lsysopy -Wl,-rpath=. 

shared_O3: compile_shared
	$(CC)  -L. -I. $(SRC) -O3 -o $(OUT_SHARED_O3) -lsysopy -Wl,-rpath=. 

shared_Os: compile_shared
	$(CC)  -L. -I. $(SRC) -Os -o $(OUT_SHARED_Os) -lsysopy -Wl,-rpath=. 

dynamic_O:
	$(CC) -o $(OUT_DYNAMIC_O) -O $(SRC) -ldl -D DYNAMIC

dynamic_O3:
	$(CC) -o $(OUT_DYNAMIC_O3) -O3 $(SRC) -ldl -D DYNAMIC

dynamic_Os:
	$(CC) -o $(OUT_DYNAMIC_Os) -Os $(SRC) -ldl -D DYNAMIC

clean_raport:
	echo "" > raport3b.txt

test_static_o: static_0
	echo "--------------------STATIC LIBRARY -O --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_static_o3: static_03
	echo "--------------------STATIC LIBRARY -O3 --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_static_os: static_0s
	echo "--------------------STATIC LIBRARY -Os --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_STATIC_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_shared_o: shared_O
	echo "--------------------SHARED LIBRARY -O --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_shared_o3: shared_O3
	echo "--------------------SHARED LIBRARY -O3 --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_shared_os: shared_Os
	echo "--------------------SHARED LIBRARY -Os --------------------" $(REDIRECT)
	#small comparison test
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_SHARED_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_dynamic_o: dynamic_O
	#small comparison test
	echo "--------------------DYNAMIC LIBRARY -O --------------------" $(REDIRECT)
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_dynamic_o3: dynamic_O3
	#small comparison test
	echo "--------------------DYNAMIC LIBRARY -O3 --------------------" $(REDIRECT)
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_O3) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test_dynamic_os: dynamic_Os
	#small comparison test
	echo "--------------------DYNAMIC LIBRARY -Os --------------------" $(REDIRECT)
	echo "--------------------SMALL COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(REDIRECT)
	#small block removal test
	echo "--------------------SMALL BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 1 compare_pairs $(FILES)a.txt:$(FILES)b.txt remove_block 0 $(REDIRECT)
	#medium comparison test
	echo "--------------------MEDIUM COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(REDIRECT)
	#medium block removal test
	echo "--------------------MEDIUM BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 4 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 remove_block 3 $(REDIRECT)
	#big test
	echo "--------------------BIG COMPARISON TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 $(REDIRECT)
	#big block removal test
	echo "--------------------BIG BLOCK REMOVAL TEST--------------------" $(REDIRECT)
	./$(OUT_DYNAMIC_Os) create_table 8 compare_pairs $(FILES)a.txt:$(FILES)b.txt $(FILES)one1:$(FILES)one2 $(FILES)two1:$(FILES)two2 $(FILES)three1:$(FILES):$(FILES)three2 $(FILES)four1:$(FILES)four2 $(FILES)five1:$(FILES)five2 $(FILES)six1:$(FILES)six2 $(FILES)seven1:$(FILES)seven2 remove_block 7 $(REDIRECT)

test: clean_raport test_static_o test_static_o3 test_static_os test_shared_o test_static_o3 test_shared_os test_dynamic_o test_dynamic_o3 test_dynamic_os clean

clean:
	rm -f *.o
	rm -f *.a
	rm -f tmp

clean_all: clean
	rm -f *.so
	rm -f shared
	rm -f dynamico*
	rm -f statico*
	rm -f sharedo*
