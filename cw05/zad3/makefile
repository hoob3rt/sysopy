.DEFAULT_TARGET = all

OBJDIR   = obj
BINDIR   = bin
rm       = rm -f

.PHONY: all
all:
	make -C producer
	make -C client

.PHONY: test
test: remove
	make
	@bash tester.sh bin/producer bin/client fifo

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/* test_file
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"
