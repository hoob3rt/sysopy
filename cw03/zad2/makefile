TARGET   = matrix

CC       = gcc
CFLAGS   = -Wall -std=c11 -ggdb -pipe -O3 -I.
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
	make -C generator/
	make -C tests/

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test
test: remove 
	make
	./$(BINDIR)/generator $(BINDIR)/ 7 14 5
	./$(BINDIR)/matrix $(BINDIR)/lista 4 3 2
	bash tester.sh $(BINDIR)/test_suite $(BINDIR)/matrices

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"

