TARGET   = test_suite

CC       = gcc
CFLAGS   = -Wall -std=c11 -ggdb -pipe -O3 -I.
LINKER   = gcc
LFLAGS   = -Wall -I. -lm

SRCDIR   = src
OBJDIR   = ../obj/generator
BINDIR   = ../bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p ../bin/
	$(LINKER) $(OBJECTS) ../obj/matrix_func.o $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p ../obj/generator
	$(CC) $(CFLAGS) -c $< -o $@
