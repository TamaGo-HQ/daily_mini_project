# Makefile for Q15.16 Fixed-Point Library - Windows Compatible
# Day 2 of 30-day Challenge - tama_GO
# Professional Directory Structure
# ===============================================

# Detect OS for cross-platform compatibility
ifeq ($(OS),Windows_NT)
    # Windows settings
    RM = del /Q
    RMDIR = rmdir /S /Q
    MKDIR = if not exist $(1) mkdir $(1)
    CP = copy
    PATHSEP = \\
    EXE_EXT = .exe
    LIB_EXT = .dll
    STAT_LIB_EXT = .a
    NULL_DEV = nul
else
    # Unix/Linux settings
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(1)
    CP = cp
    PATHSEP = /
    EXE_EXT =
    LIB_EXT = .so
    STAT_LIB_EXT = .a
    NULL_DEV = /dev/null
endif

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g -Iinclude
LDFLAGS = -lm

# Directory structure
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

# Source files
LIB_SOURCES = $(SRC_DIR)$(PATHSEP)q1516.c
TEST_SOURCES = $(TEST_DIR)$(PATHSEP)test_q1516.c
HEADERS = $(INCLUDE_DIR)$(PATHSEP)q1516.h

# Object files  
LIB_OBJECTS = $(BUILD_DIR)$(PATHSEP)q1516.o
TEST_OBJECTS = $(BUILD_DIR)$(PATHSEP)test_q1516.o

# Targets
STATIC_LIB = $(LIB_DIR)$(PATHSEP)libq1516$(STAT_LIB_EXT)
SHARED_LIB = $(LIB_DIR)$(PATHSEP)libq1516$(LIB_EXT)
TEST_EXECUTABLE = $(BIN_DIR)$(PATHSEP)test_q1516$(EXE_EXT)
DEMO_EXECUTABLE = $(BIN_DIR)$(PATHSEP)demo_q1516$(EXE_EXT)

# Default target
all: directories $(STATIC_LIB) $(SHARED_LIB) $(TEST_EXECUTABLE) $(DEMO_EXECUTABLE)

# Create all necessary directories
directories:
ifeq ($(OS),Windows_NT)
	@if not exist $(SRC_DIR) mkdir $(SRC_DIR)
	@if not exist $(INCLUDE_DIR) mkdir $(INCLUDE_DIR)
	@if not exist $(TEST_DIR) mkdir $(TEST_DIR)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(LIB_DIR) mkdir $(LIB_DIR)
else
	@mkdir -p $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR) $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)
endif

# Setup project structure (run once to create files)
setup: directories
	@echo Setting up Q15.16 project structure...
ifeq ($(OS),Windows_NT)
	@if not exist $(INCLUDE_DIR)$(PATHSEP)q1516.h ( \
		echo Creating $(INCLUDE_DIR)$(PATHSEP)q1516.h... && \
		if exist q1516.h copy q1516.h $(INCLUDE_DIR)$(PATHSEP) 2>$(NULL_DEV) || echo Please create $(INCLUDE_DIR)$(PATHSEP)q1516.h manually \
	)
	@if not exist $(SRC_DIR)$(PATHSEP)q1516.c ( \
		echo Creating $(SRC_DIR)$(PATHSEP)q1516.c... && \
		if exist q1516.c copy q1516.c $(SRC_DIR)$(PATHSEP) 2>$(NULL_DEV) || echo Please create $(SRC_DIR)$(PATHSEP)q1516.c manually \
	)
	@if not exist $(TEST_DIR)$(PATHSEP)test_q1516.c ( \
		echo Creating $(TEST_DIR)$(PATHSEP)test_q1516.c... && \
		if exist test_q1516.c copy test_q1516.c $(TEST_DIR)$(PATHSEP) 2>$(NULL_DEV) || echo Please create $(TEST_DIR)$(PATHSEP)test_q1516.c manually \
	)
else
	@if [ ! -f $(INCLUDE_DIR)/q1516.h ]; then \
		echo "Creating $(INCLUDE_DIR)/q1516.h..."; \
		cp q1516.h $(INCLUDE_DIR)/ 2>/dev/null || echo "Please create $(INCLUDE_DIR)/q1516.h manually"; \
	fi
	@if [ ! -f $(SRC_DIR)/q1516.c ]; then \
		echo "Creating $(SRC_DIR)/q1516.c..."; \
		cp q1516.c $(SRC_DIR)/ 2>/dev/null || echo "Please create $(SRC_DIR)/q1516.c manually"; \
	fi
	@if [ ! -f $(TEST_DIR)/test_q1516.c ]; then \
		echo "Creating $(TEST_DIR)/test_q1516.c..."; \
		cp test_q1516.c $(TEST_DIR)/ 2>/dev/null || echo "Please create $(TEST_DIR)/test_q1516.c manually"; \
	fi
endif
	@echo Project structure created!
	@echo.
	@echo Directory structure:
	@echo ^|-- include^/          # Header files
	@echo ^|   ^`-- q1516.h
	@echo ^|-- src^/              # Source files
	@echo ^|   ^`-- q1516.c
	@echo ^|-- test^/             # Test files
	@echo ^|   ^`-- test_q1516.c
	@echo ^|-- build^/            # Object files
	@echo ^|-- lib^/              # Libraries
	@echo ^|-- bin^/              # Executables
	@echo ^`-- Makefile

# Compile library source to object file
$(BUILD_DIR)$(PATHSEP)q1516.o: $(SRC_DIR)$(PATHSEP)q1516.c $(INCLUDE_DIR)$(PATHSEP)q1516.h | directories
	$(CC) $(CFLAGS) -c $(SRC_DIR)$(PATHSEP)q1516.c -o $(BUILD_DIR)$(PATHSEP)q1516.o

# Create static library
$(STATIC_LIB): $(LIB_OBJECTS) | directories
	ar rcs $(STATIC_LIB) $(LIB_OBJECTS)
	@echo Static library created: $(STATIC_LIB)

# Create shared library
$(SHARED_LIB): $(LIB_OBJECTS) | directories
ifeq ($(OS),Windows_NT)
	$(CC) -shared -fPIC $(LIB_OBJECTS) -o $(SHARED_LIB) -Wl,--out-implib,$(LIB_DIR)$(PATHSEP)libq1516.dll.a
else
	$(CC) -shared -fPIC $(LIB_OBJECTS) -o $(SHARED_LIB)
endif
	@echo Shared library created: $(SHARED_LIB)

# Compile test source
$(BUILD_DIR)$(PATHSEP)test_q1516.o: $(TEST_DIR)$(PATHSEP)test_q1516.c $(INCLUDE_DIR)$(PATHSEP)q1516.h | directories
	$(CC) $(CFLAGS) -c $(TEST_DIR)$(PATHSEP)test_q1516.c -o $(BUILD_DIR)$(PATHSEP)test_q1516.o

# Build test executable
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(STATIC_LIB) | directories
	$(CC) $(TEST_OBJECTS) $(STATIC_LIB) $(LDFLAGS) -o $(TEST_EXECUTABLE)
	@echo Test executable created: $(TEST_EXECUTABLE)

# Build demo executable
$(DEMO_EXECUTABLE): $(BUILD_DIR)$(PATHSEP)demo.o $(STATIC_LIB) | directories
	$(CC) $(BUILD_DIR)$(PATHSEP)demo.o $(STATIC_LIB) $(LDFLAGS) -o $(DEMO_EXECUTABLE)
	@echo Demo executable created: $(DEMO_EXECUTABLE)

# Create demo source file
$(BUILD_DIR)$(PATHSEP)demo.o: demo_source | directories
	$(CC) $(CFLAGS) -c demo.c -o $(BUILD_DIR)$(PATHSEP)demo.o

# Generate demo source file
demo_source:
	@echo Creating demo.c...
ifeq ($(OS),Windows_NT)
	@echo #include "q1516.h" > demo.c
	@echo #include ^<stdio.h^> >> demo.c
	@echo. >> demo.c
	@echo int main() { >> demo.c
	@echo     printf("Q15.16 Fixed-Point Library Demo\n"); >> demo.c
	@echo     printf("================================\n\n"); >> demo.c
	@echo. >> demo.c
	@echo     // Create some fixed-point numbers >> demo.c
	@echo     q1516_t a = q1516_from_float(3.14159f); >> demo.c
	@echo     q1516_t b = q1516_from_float(2.0f); >> demo.c
	@echo     q1516_t c = q1516_from_int(7); >> demo.c
	@echo. >> demo.c
	@echo     // Perform operations >> demo.c
	@echo     q1516_t sum = q1516_add(a, b); >> demo.c
	@echo     q1516_t product = q1516_multiply(a, b); >> demo.c
	@echo     q1516_t quotient = q1516_divide(c, b); >> demo.c
	@echo. >> demo.c
	@echo     // Display results >> demo.c
	@echo     q1516_print("π (pi)", a); >> demo.c
	@echo     q1516_print("2.0", b); >> demo.c
	@echo     q1516_print("7", c); >> demo.c
	@echo     printf("\nArithmetic operations:\n"); >> demo.c
	@echo     q1516_print("π + 2.0", sum); >> demo.c
	@echo     q1516_print("π × 2.0", product); >> demo.c
	@echo     q1516_print("7 ÷ 2.0", quotient); >> demo.c
	@echo. >> demo.c
	@echo     printf("\nDetailed view of π:\n"); >> demo.c
	@echo     q1516_print_detailed("π (pi)", a); >> demo.c
	@echo. >> demo.c
	@echo     return 0; >> demo.c
	@echo } >> demo.c
else
	@echo '#include "q1516.h"' > demo.c
	@echo '#include <stdio.h>' >> demo.c
	@echo '' >> demo.c
	@echo 'int main() {' >> demo.c
	@echo '    printf("Q15.16 Fixed-Point Library Demo\\n");' >> demo.c
	@echo '    printf("================================\\n\\n");' >> demo.c
	@echo '' >> demo.c
	@echo '    // Create some fixed-point numbers' >> demo.c
	@echo '    q1516_t a = q1516_from_float(3.14159f);' >> demo.c
	@echo '    q1516_t b = q1516_from_float(2.0f);' >> demo.c
	@echo '    q1516_t c = q1516_from_int(7);' >> demo.c
	@echo '' >> demo.c
	@echo '    // Perform operations' >> demo.c
	@echo '    q1516_t sum = q1516_add(a, b);' >> demo.c
	@echo '    q1516_t product = q1516_multiply(a, b);' >> demo.c
	@echo '    q1516_t quotient = q1516_divide(c, b);' >> demo.c
	@echo '' >> demo.c
	@echo '    // Display results' >> demo.c
	@echo '    q1516_print("π (pi)", a);' >> demo.c
	@echo '    q1516_print("2.0", b);' >> demo.c
	@echo '    q1516_print("7", c);' >> demo.c
	@echo '    printf("\\nArithmetic operations:\\n");' >> demo.c
	@echo '    q1516_print("π + 2.0", sum);' >> demo.c
	@echo '    q1516_print("π × 2.0", product);' >> demo.c
	@echo '    q1516_print("7 ÷ 2.0", quotient);' >> demo.c
	@echo '' >> demo.c
	@echo '    printf("\\nDetailed view of π:\\n");' >> demo.c
	@echo '    q1516_print_detailed("π (pi)", a);' >> demo.c
	@echo '' >> demo.c
	@echo '    return 0;' >> demo.c
	@echo '}' >> demo.c
endif

# Run tests
test: $(TEST_EXECUTABLE)
	@echo Running comprehensive test suite...
	@echo ===================================
	$(TEST_EXECUTABLE)

# Run demo
demo: $(DEMO_EXECUTABLE)
	@echo Running demo program...
	@echo =======================
	$(DEMO_EXECUTABLE)

# Clean build files
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(BUILD_DIR) $(RMDIR) $(BUILD_DIR) 2>$(NULL_DEV)
	@if exist $(BIN_DIR) $(RMDIR) $(BIN_DIR) 2>$(NULL_DEV)
	@if exist $(LIB_DIR) $(RMDIR) $(LIB_DIR) 2>$(NULL_DEV)
	@if exist demo.c $(RM) demo.c 2>$(NULL_DEV)
else
	$(RMDIR) $(BUILD_DIR)
	$(RMDIR) $(BIN_DIR)
	$(RMDIR) $(LIB_DIR)
	$(RM) demo.c
endif
	@echo Cleaned build files

# Clean everything including generated directories
distclean: clean
ifeq ($(OS),Windows_NT)
	@if exist $(SRC_DIR) $(RMDIR) $(SRC_DIR) 2>$(NULL_DEV)
	@if exist $(INCLUDE_DIR) $(RMDIR) $(INCLUDE_DIR) 2>$(NULL_DEV)
	@if exist $(TEST_DIR) $(RMDIR) $(TEST_DIR) 2>$(NULL_DEV)
else
	$(RMDIR) $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR)
endif
	@echo Cleaned all generated files and directories

# Clean and rebuild everything
rebuild: clean all

# Check project structure
check-structure:
	@echo Checking project structure...
	@echo =============================
ifeq ($(OS),Windows_NT)
	@if exist $(INCLUDE_DIR)$(PATHSEP)q1516.h (echo ✓ $(INCLUDE_DIR)$(PATHSEP)q1516.h) else (echo ✗ $(INCLUDE_DIR)$(PATHSEP)q1516.h missing)
	@if exist $(SRC_DIR)$(PATHSEP)q1516.c (echo ✓ $(SRC_DIR)$(PATHSEP)q1516.c) else (echo ✗ $(SRC_DIR)$(PATHSEP)q1516.c missing)
	@if exist $(TEST_DIR)$(PATHSEP)test_q1516.c (echo ✓ $(TEST_DIR)$(PATHSEP)test_q1516.c) else (echo ✗ $(TEST_DIR)$(PATHSEP)test_q1516.c missing)
	@echo.
	@echo Directory tree:
	@dir /AD /S 2>$(NULL_DEV) || echo Directory listing not available
else
	@if [ -f $(INCLUDE_DIR)/q1516.h ]; then echo "✓ $(INCLUDE_DIR)/q1516.h"; else echo "✗ $(INCLUDE_DIR)/q1516.h missing"; fi
	@if [ -f $(SRC_DIR)/q1516.c ]; then echo "✓ $(SRC_DIR)/q1516.c"; else echo "✗ $(SRC_DIR)/q1516.c missing"; fi
	@if [ -f $(TEST_DIR)/test_q1516.c ]; then echo "✓ $(TEST_DIR)/test_q1516.c"; else echo "✗ $(TEST_DIR)/test_q1516.c missing"; fi
	@echo ""
	@echo "Directory tree:"
	@tree . 2>/dev/null || find . -type d | sed 's|[^/]*/|  |g'
endif

# Show project information
info:
	@echo Q15.16 Fixed-Point Library
	@echo ==========================
	@echo Compiler: $(CC)
	@echo Flags: $(CFLAGS)
	@echo.
	@echo Directories:
	@echo   Source: $(SRC_DIR)$(PATHSEP)
	@echo   Headers: $(INCLUDE_DIR)$(PATHSEP)
	@echo   Tests: $(TEST_DIR)$(PATHSEP)
	@echo   Build: $(BUILD_DIR)$(PATHSEP)
	@echo   Libraries: $(LIB_DIR)$(PATHSEP)
	@echo   Binaries: $(BIN_DIR)$(PATHSEP)
	@echo.
	@echo Targets:
	@echo   Static lib: $(STATIC_LIB)
	@echo   Shared lib: $(SHARED_LIB)
	@echo   Test exe: $(TEST_EXECUTABLE)
	@echo   Demo exe: $(DEMO_EXECUTABLE)

# Show help
help:
	@echo Q15.16 Fixed-Point Library Makefile - Windows Compatible
	@echo =======================================================
	@echo.
	@echo Setup commands:
	@echo   setup          - Create proper directory structure
	@echo   check-structure - Verify all files are in place
	@echo.
	@echo Build commands:
	@echo   all            - Build everything (default)
	@echo   test           - Build and run comprehensive tests  
	@echo   demo           - Build and run demo program
	@echo   clean          - Remove build files
	@echo   rebuild        - Clean and rebuild everything
	@echo   distclean      - Remove all generated files/dirs
	@echo.
	@echo Development commands:
	@echo   info           - Show build information
	@echo.
	@echo Quick start:
	@echo   make setup     # Create directory structure
	@echo   make test      # Build and run tests

# Debug and release variants
debug: CFLAGS += -DDEBUG -g3 -O0
debug: all
	@echo Debug build complete

release: CFLAGS += -DNDEBUG -O3
release: clean all
	@echo Release build complete

# Phony targets
.PHONY: all test demo clean distclean rebuild setup check-structure 
.PHONY: info help debug release directories demo_source