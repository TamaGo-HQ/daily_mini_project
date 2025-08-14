# Makefile for Bit Operations Library
# Author: Your Name
# Date: 2025

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include

# Directories
INCLUDE_DIR = include
TESTS_DIR = tests
EXAMPLES_DIR = examples

# Target executables
TEST_TARGET = test_bit_ops
EXAMPLE_TARGET = example_usage

# Source files
TEST_SOURCES = $(TESTS_DIR)/test_bit_ops.c
EXAMPLE_SOURCES = $(EXAMPLES_DIR)/example_usage.c

# Default target (runs when you just type 'make')
all: $(TEST_TARGET)

# Rule to build the test executable
$(TEST_TARGET): $(TEST_SOURCES) $(INCLUDE_DIR)/bit_ops.h
	@echo "Compiling test suite..."
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)
	@echo "✓ Test executable created: $(TEST_TARGET)"

# Rule to build example program
example: $(EXAMPLE_TARGET)

$(EXAMPLE_TARGET): $(EXAMPLE_SOURCES) $(INCLUDE_DIR)/bit_ops.h
	@echo "Compiling example program..."
	$(CC) $(CFLAGS) $(EXAMPLE_SOURCES) -o $(EXAMPLE_TARGET)
	@echo "✓ Example executable created: $(EXAMPLE_TARGET)"

# Rule to run tests
test: $(TEST_TARGET)
	@echo "Running test suite..."
	@./$(TEST_TARGET)

# Rule to run example
run-example: $(EXAMPLE_TARGET)
	@echo "Running example program..."
	@./$(EXAMPLE_TARGET)

# Rule to clean up compiled files
clean:
	@echo "Cleaning up..."
	rm -f $(TEST_TARGET) $(EXAMPLE_TARGET)
	@echo "✓ Cleanup complete"

# Rule to show help
help:
	@echo "Available targets:"
	@echo "  all        - Build test suite (default)"
	@echo "  test       - Build and run test suite"
	@echo "  example    - Build example program"
	@echo "  run-example- Build and run example program"
	@echo "  clean      - Remove all compiled files"
	@echo "  help       - Show this help message"

# Declare phony targets (not actual files)
.PHONY: all test example run-example clean help