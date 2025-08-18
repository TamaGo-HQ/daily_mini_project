CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0 -Iinclude
SRCDIR = src
TESTDIR = tests
EXAMPLEDIR = examples
INCLUDEDIR = include

# Source files
SOURCES = $(SRCDIR)/circular_buffer.c
OBJECTS = $(SOURCES:.c=.o)

# Test executables
TEST_BASIC = $(TESTDIR)/test_basic
TESTS = $(TEST_BASIC)

# Default target
all: $(TESTS)

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Test executables
$(TEST_BASIC): $(TESTDIR)/test_basic.c $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Run tests
test: $(TESTS)
	@echo "Running tests..."
	./$(TEST_BASIC)

# Clean build files
clean:
	rm -f $(SRCDIR)/*.o $(TESTS)

# Check for memory leaks (requires valgrind)
memcheck: $(TEST_BASIC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BASIC)

.PHONY: all test clean memcheck