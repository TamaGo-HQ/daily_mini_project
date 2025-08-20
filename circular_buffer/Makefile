CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0 -Iinclude
SRCDIR = src
TESTDIR = tests

# Source files
SOURCES = $(SRCDIR)/circular_buffer.c
OBJECTS = $(SOURCES:.c=.o)

# Only the threadsafe test
TEST_THREADSAFE = $(TESTDIR)/test_threadsafe

# Default target
all: $(TEST_THREADSAFE)

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Test executable
$(TEST_THREADSAFE): $(TESTDIR)/test_threadsafe.c $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Run the threadsafe test
test: $(TEST_THREADSAFE)
	@echo "Running threadsafe test..."
	./$(TEST_THREADSAFE)

# Clean build files
clean:
	rm -f $(SRCDIR)/*.o $(TEST_THREADSAFE)

# Memory check using valgrind
memcheck: $(TEST_THREADSAFE)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_THREADSAFE)

.PHONY: all test clean memcheck
