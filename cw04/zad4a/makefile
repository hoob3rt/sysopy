.DEFAULT_GOAL := all

OBJDIR  = obj
BINDIR = bin
rm = rm -f

.PHONY: all
all:
	@make -C catcher/
	@make -C sender/

.PHONY: test
test: all
	@bash tester.sh

.PHONY: clean
clean:
	@$(rm) -r $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -r $(BINDIR)/*
	@echo "Executable removed!"
