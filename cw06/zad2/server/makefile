TARGET   = server

CC       = gcc
CFLAGS   = -Wall -lrt
#-std=c11 -ggdb -pipe -I. 
LINKER   = gcc
LFLAGS   = -Wall -I. -lm -lrt

SRCDIR   = src
OBJDIR   = ../obj/server
BINDIR   = ../bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)/
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)/
	$(CC) $(CFLAGS) -c $< -o $@
