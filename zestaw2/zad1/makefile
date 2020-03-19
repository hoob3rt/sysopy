.DEFUALT_GOAL := out

src = $(wildcard *.c)
obj = $(src:.c=.o)

REDIRECT = >> wyniki.txt
CC = gcc -Wall -std=c11 -ggdb -pipe -O3

out: $(obj)
	$(CC) -o $@ $^

test: out
	echo "--------------------1 byte--------------------" $(REDIRECT)
	echo "-----generate 100 elements-----" $(REDIRECT)
	./$^ generate generated 100 1 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 100 1 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  100 1 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 100 1 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 100 1 sys $(REDIRECT)
	echo "--------------------4 bytes--------------------" $(REDIRECT)
	echo "-----generate 500-----" $(REDIRECT)
	./$^ generate generated 500 4 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 500 4 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  500 4 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 500 4 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 500 4 sys $(REDIRECT)
	echo "--------------------512 bytes--------------------" $(REDIRECT)
	echo "-----generate 1000-----" $(REDIRECT)
	./$^ generate generated 1000 512 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 1000 512 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  1000 512 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 1000 512 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 1000 512 sys $(REDIRECT)
	echo "--------------------1024 bytes--------------------" $(REDIRECT)
	echo "-----generate 10000-----" $(REDIRECT)
	./$^ generate generated 1000 1024 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 1000 1024 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  1000 1024 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 1000 1024 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 1000 1024 sys $(REDIRECT)
	echo "--------------------4096 bytes--------------------" $(REDIRECT)
	echo "-----generate 20000-----" $(REDIRECT)
	./$^ generate generated 20000 4096 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 20000 4096 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  20000 4096 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 20000 4096 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 20000 4096 sys $(REDIRECT)
	echo "--------------------8192 bytes--------------------" $(REDIRECT)
	echo "-----generate 50000-----" $(REDIRECT)
	./$^ generate generated 50000 8192 $(REDIRECT)
	echo "-----lib copy-----" $(REDIRECT)
	./$^ copy generated libcopy 50000 8192 lib $(REDIRECT)
	echo "-----sys copy-----" $(REDIRECT)
	./$^ copy generated syscopy  50000 8192 sys $(REDIRECT)
	echo "-----lib sort-----" $(REDIRECT)
	./$^ sort libcopy 50000 8192 lib $(REDIRECT)
	echo "-----sys sort-----" $(REDIRECT)
	./$^ sort syscopy 50000 8192 sys $(REDIRECT)
	echo "\nfunkcje systemowe dzialaja szybciej" $(REDIRECT)
	@$(MAKE) --no-print-directory clean_all

.PHONY: clean
.PHONY: clean_all
clean:
	rm -f $(obj) out 
clean_all: clean
	rm -f syscopy libcopy generated
