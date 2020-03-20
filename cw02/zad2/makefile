.DEFUALT_GOAL := out

src = $(wildcard *.c)
obj = $(src:.c=.o)

CC = gcc -Wall -std=c11 -ggdb -pipe -O3

out: $(obj)
	$(CC) -o $@ $^

test: out
	bash generator.sh
	./$^ test 
	./$^ test -maxdepth 100
	@$(MAKE) --no-print-directory clean

.PHONY: clean
clean:
	rm -f $(obj) out
	rm -rf test
