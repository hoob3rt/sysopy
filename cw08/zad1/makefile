TARGET   = histogram

CC       = gcc
CFLAGS   = -Wall -ggdb -pipe -I. -lpthread -lm
LINKER   = gcc
LFLAGS   = -Wall -ggdb -pipe -I. -lpthread -lm

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
RESULTSDIR = results

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

IMAGE = images/f18.ascii.pgm

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p bin/
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test
test: remove
	make
	./$(BINDIR)/$(TARGET) 1 sign $(IMAGE) $(RESULTSDIR)/sign1.txt
	./$(BINDIR)/$(TARGET) 1 block $(IMAGE) $(RESULTSDIR)/block1.txt
	./$(BINDIR)/$(TARGET) 1 interleaved $(IMAGE) $(RESULTSDIR)/interleaved1.txt
	./$(BINDIR)/$(TARGET) 2 sign $(IMAGE) $(RESULTSDIR)/sign2.txt
	./$(BINDIR)/$(TARGET) 2 block $(IMAGE) $(RESULTSDIR)/block2.txt
	./$(BINDIR)/$(TARGET) 2 interleaved $(IMAGE) $(RESULTSDIR)/interleaved2.txt
	./$(BINDIR)/$(TARGET) 4 sign $(IMAGE) $(RESULTSDIR)/sign4.txt
	./$(BINDIR)/$(TARGET) 4 block $(IMAGE) $(RESULTSDIR)/block4.txt
	./$(BINDIR)/$(TARGET) 4 interleaved $(IMAGE) $(RESULTSDIR)/interleaved4.txt
	./$(BINDIR)/$(TARGET) 8 sign $(IMAGE) $(RESULTSDIR)/sign8.txt
	./$(BINDIR)/$(TARGET) 8 block $(IMAGE) $(RESULTSDIR)/block8.txt
	./$(BINDIR)/$(TARGET) 8 interleaved $(IMAGE) $(RESULTSDIR)/interleaved8.txt

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/* test_file
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"
