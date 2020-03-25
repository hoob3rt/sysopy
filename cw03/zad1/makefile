TARGET   = ls

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

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test
test: 
	bash generator.sh
	./bin/ls test/
	@$(MAKE) --no-print-directory clean

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) -r test || true
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

