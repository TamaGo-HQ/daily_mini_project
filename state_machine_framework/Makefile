# ====================================
# State Machine Framework Makefile
# ====================================

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS = 

# Project directories
SRC_DIR = src
INCLUDE_DIR = include
EXAMPLES_DIR = exemples
TESTS_DIR = tests
BUILD_DIR = build

# Source files
FRAMEWORK_SOURCES = $(SRC_DIR)/state_machine.c
FRAMEWORK_OBJECTS = $(BUILD_DIR)/state_machine.o

# Example executables
TRAFFIC_LIGHT_EXEC = $(BUILD_DIR)/traffic_light
TRAFFIC_LIGHT_SOURCES = $(EXAMPLES_DIR)/traffic_light.c

# Test executables (for future use)
TEST_EXEC = $(BUILD_DIR)/test_state_machine

# Include paths
INCLUDES = -I$(INCLUDE_DIR)

# Default target
.PHONY: all
all: $(TRAFFIC_LIGHT_EXEC)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile framework source
$(BUILD_DIR)/state_machine.o: $(SRC_DIR)/state_machine.c $(INCLUDE_DIR)/state_machine.h | $(BUILD_DIR)
	@echo "🔨 Compiling state machine framework..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build traffic light example
$(TRAFFIC_LIGHT_EXEC): $(FRAMEWORK_OBJECTS) $(TRAFFIC_LIGHT_SOURCES) | $(BUILD_DIR)
	@echo "🚦 Building traffic light example..."
	$(CC) $(CFLAGS) $(INCLUDES) $(FRAMEWORK_OBJECTS) $(TRAFFIC_LIGHT_SOURCES) -o $@
	@echo "✅ Traffic light example built successfully!"

# Run traffic light example
.PHONY: run
run: $(TRAFFIC_LIGHT_EXEC)
	@echo " Running traffic light example..."
	@echo "=================================="
	./$(TRAFFIC_LIGHT_EXEC)

# Debug version with additional flags
.PHONY: debug
debug: CFLAGS += -DDEBUG -O0
debug: $(TRAFFIC_LIGHT_EXEC)
	@echo " Debug version built!"

# Release version with optimizations
.PHONY: release
release: CFLAGS += -O2 -DNDEBUG
release: $(TRAFFIC_LIGHT_EXEC)
	@echo " Release version built!"

# Build and run in one command
.PHONY: build-run
build-run: $(TRAFFIC_LIGHT_EXEC)
	@echo " Building and running traffic light example..."
	@echo "================================================"
	./$(TRAFFIC_LIGHT_EXEC)

# Static analysis with additional warnings
.PHONY: analyze
analyze: CFLAGS += -Wshadow -Wcast-align -Wwrite-strings -Wredundant-decls -Winline -Wno-long-long -Wconversion
analyze: $(TRAFFIC_LIGHT_EXEC)
	@echo " Static analysis build completed!"

# Memory check with valgrind (if available)
.PHONY: memcheck
memcheck: $(TRAFFIC_LIGHT_EXEC)
	@echo " Running memory check..."
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Note: Use Ctrl+C to exit the program during memory check"; \
		echo "Running for 10 seconds then will send quit signal..."; \
		timeout 10s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TRAFFIC_LIGHT_EXEC) || true; \
	else \
		echo " Valgrind not found. Install it for memory checking."; \
	fi

# Create a simple test framework (basic functionality test)
.PHONY: test
test: $(FRAMEWORK_OBJECTS) | $(BUILD_DIR)
	@echo " Running basic functionality tests..."
	@echo '#include "../include/state_machine.h"' > $(BUILD_DIR)/basic_test.c
	@echo '#include <stdio.h>' >> $(BUILD_DIR)/basic_test.c
	@echo '#include <assert.h>' >> $(BUILD_DIR)/basic_test.c
	@echo 'int main() {' >> $(BUILD_DIR)/basic_test.c
	@echo '    state_machine_t sm;' >> $(BUILD_DIR)/basic_test.c
	@echo '    printf(" Basic compilation test passed!\\n");' >> $(BUILD_DIR)/basic_test.c
	@echo '    return 0;' >> $(BUILD_DIR)/basic_test.c
	@echo '}' >> $(BUILD_DIR)/basic_test.c
	$(CC) $(CFLAGS) $(INCLUDES) $(FRAMEWORK_OBJECTS) $(BUILD_DIR)/basic_test.c -o $(BUILD_DIR)/basic_test
	./$(BUILD_DIR)/basic_test
	@rm -f $(BUILD_DIR)/basic_test.c $(BUILD_DIR)/basic_test

# Clean build artifacts
.PHONY: clean
clean:
	@echo " Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	@echo " Clean completed!"

# Install framework headers and library (optional)
.PHONY: install
install: $(FRAMEWORK_OBJECTS)
	@echo " Installing state machine framework..."
	@echo "Note: This would typically install to /usr/local/lib and /usr/local/include"
	@echo "For now, framework is ready to use from current directory"

# Show project structure
.PHONY: tree
tree:
	@echo " Project structure:"
	@echo "state_machine_framework/"
	@find . -type f -name "*.c" -o -name "*.h" -o -name "Makefile" | grep -v "build/" | sort | sed 's|^./|  |'
	@if [ -d $(BUILD_DIR) ]; then \
		echo "  build/ (generated)"; \
		find $(BUILD_DIR) -type f | sort | sed 's|^|    |'; \
	fi

# Help target
.PHONY: help
help:
	@echo "🛠️  State Machine Framework Build System"
	@echo "========================================"
	@echo ""
	@echo "Available targets:"
	@echo "  all          - Build traffic light example (default)"
	@echo "  run          - Build and run traffic light example"
	@echo "  build-run    - Same as run (alias)"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  analyze      - Build with extra static analysis warnings"
	@echo "  memcheck     - Run with valgrind memory checking (if available)"
	@echo "  test         - Run basic functionality tests"
	@echo "  clean        - Remove all build artifacts"
	@echo "  tree         - Show project file structure"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build traffic light example"
	@echo "  make run          # Build and run immediately"
	@echo "  make debug run    # Build debug version and run"
	@echo "  make clean all    # Clean and rebuild"
	@echo ""

# Check if all required files exist
.PHONY: check
check:
	@echo " Checking project files..."
	@echo -n "  state_machine.h: "
	@if [ -f $(INCLUDE_DIR)/state_machine.h ]; then echo ""; else echo " Missing!"; fi
	@echo -n "  state_machine.c: "
	@if [ -f $(SRC_DIR)/state_machine.c ]; then echo ""; else echo " Missing!"; fi
	@echo -n "  traffic_light.c: "
	@if [ -f $(EXAMPLES_DIR)/traffic_light.c ]; then echo ""; else echo " Missing!"; fi
	@echo -n "  Makefile: "
	@if [ -f Makefile ]; then echo ""; else echo " Missing!"; fi

# Show compiler version and flags
.PHONY: info
info:
	@echo "🔧 Build Information"
	@echo "==================="
	@echo "Compiler: $(CC)"
	@$(CC) --version | head -1
	@echo "Flags: $(CFLAGS)"
	@echo "Include paths: $(INCLUDES)"
	@echo "Build directory: $(BUILD_DIR)"

# Development helper - watch for changes and rebuild
.PHONY: watch
watch:
	@echo "👀 Watching for file changes..."
	@echo "Press Ctrl+C to stop"
	@if command -v inotifywait >/dev/null 2>&1; then \
		while true; do \
			inotifywait -e modify $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h $(EXAMPLES_DIR)/*.c 2>/dev/null && \
			echo "📝 File changed, rebuilding..." && \
			make all; \
		done \
	else \
		echo "❌ inotify-tools not installed. Install it to use watch mode."; \
	fi