TARGET   = barber_shop

CC       = gcc
CFLAGS   = -Wall -std=c11 -ggdb -pipe -I. -lpthread -lm
LINKER   = gcc
LFLAGS   = -Wall -std=c11 -ggdb -pipe -I. -lpthread -lm

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
	./$(BINDIR)/$(TARGET) 5 20

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/* test_file
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"
