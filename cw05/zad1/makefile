TARGET   = sort

CC       = gcc
CFLAGS   = -Wall -std=c11 -ggdb -pipe -I. 
LINKER   = gcc
LFLAGS   = -Wall -I. -lm

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p bin/
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test
test: remove
	make
	@bash generator.sh test_file 20
	./$(BINDIR)/sort test_file

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/* test_file
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"
