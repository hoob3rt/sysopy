.DEFAULT-TARGET=all

SERVER = server
PLAYER = player

CC       = gcc
CFLAGS   = -Wall -std=c11 -ggdb -pipe -I. -lpthread
LINKER   = gcc
LFLAGS   = -Wall -std=c11 -ggdb -pipe -I. -lpthread

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

all: $(BINDIR)/$(SERVER) $(BINDIR)/$(PLAYER)

$(BINDIR)/$(SERVER): $(OBJECTS)
	@mkdir -p bin/
	$(LINKER) $(OBJDIR)/$(SERVER).o $(LFLAGS) -o $@

$(BINDIR)/$(PLAYER): $(OBJECTS)
	@mkdir -p bin/
	$(LINKER) $(OBJDIR)/$(PLAYER).o $(LFLAGS) -o $@

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
