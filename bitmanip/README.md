# 🔧 Bit Manipulation Library for Embedded Systems

**Project 1 of 30** - *30 Days of C Programming for Embedded Systems*

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A macro-based bit manipulation library designed for embedded systems development. This library provides fast, inline bit operations optimized for microcontrollers and real-time applications.

##  Project Motivation

As part of my journey to master embedded systems programming and prepare for embedded software engineering roles, I embarked on a **30-projects C programming challenge**. This bit manipulation library serves as the foundation project because:

- **Fundamental Skill**: Bit manipulation is crucial in embedded systems for hardware register control, GPIO operations, and memory-efficient programming
- **Performance Critical**: Embedded systems require fast, efficient code with minimal overhead
- **Industry Standard**: Every embedded developer must master bitwise operations for hardware abstraction layers (HAL), device drivers, and real-time systems
- **Because I Want to Do it**

This project helped my understanding of:
- Macro design and safety considerations
- Professional code organization and documentation
- Comprehensive testing methodologies
- Build system management (Makefiles)

## 📋 Features

### Basic Bit Operations
- **SET_BIT(value, position)** - Set a specific bit to 1
- **CLEAR_BIT(value, position)** - Clear a specific bit to 0
- **TOGGLE_BIT(value, position)** - Toggle a specific bit
- **CHECK_BIT(value, position)** - Check if a bit is set (returns bit value)
- **IS_BIT_SET(value, position)** - Check if a bit is set (returns 0 or 1)

## 🛠️ Building and Testing

### Method 1: Using Makefile (Recommended)

```bash
# Build and run comprehensive test suite
make test

# Clean up build artifacts
make clean

# Show available targets
make help
```

### Method 2: Manual Compilation

```bash
# Compile test suite
gcc -Wall -Wextra -std=c99 -I./include tests/test_bit_ops.c -o test_bit_ops

# Run tests
./test_bit_ops
```
## Project Structure

```
bit_manipulation_lib/
├── README.md              # This documentation
├── Makefile              # Build automation
├── include/
│   └── bit_ops.h         # Main header file with all macros
├── tests/
│   └── test_bit_ops.c    # Comprehensive test suite
├── examples/
│   └── example_usage.c   # Usage examples
```

## Testing

The project includes a comprehensive test suite with **45+ test cases** covering:

### Test Categories
- **Basic Operations**: Verify each macro works correctly
- **Edge Cases**: Boundary conditions (bit 0, MSB, all zeros/ones)
- **Data Types**: Testing with uint8_t, uint16_t, uint32_t

### Test Results Example
```
Starting Bit Operations Test Suite...

=== SET_BIT Tests ===
✓ PASS: SET_BIT: Set bit 0 in uint8_t
✓ PASS: SET_BIT: Set bit 3 in uint8_t
✓ PASS: SET_BIT: Set bit 7 in uint8_t (MSB)

=== CLEAR_BIT Tests ===
✓ PASS: CLEAR_BIT: Clear bit 0 in uint8_t
✓ PASS: CLEAR_BIT: Clear bit 3 in uint8_t

=== TEST SUMMARY ===
Total Tests: 45
Passed: 45
Failed: 0
Success Rate: 100.0%
```

## 🎯 Key Technical Decisions

### Why Macros Over Functions?
- **Performance**: Zero function call overhead - critical for embedded systems
- **Inline Expansion**: Code is inserted directly at compile time
- **Real-time Systems**: Deterministic execution time

### Safety Considerations
```c
// All macros use proper parentheses to prevent operator precedence issues
#define SET_BIT(value, position) ((value) | (1U << (position)))
//                               ^     ^    ^  ^      ^      ^
//                               Prevents expression evaluation problems
```

### The do-while(0) Idiom
```c
#define MULTI_STATEMENT_MACRO() do { \
    statement1; \
    statement2; \
} while(0)
```
Used for multi-statement macros to ensure they behave like single statements in all contexts.

## 🚧 Challenges Faced and Solutions

### Challenge 1: Macro Parameter Safety
**Problem**: Macro parameters can be expressions, leading to unexpected behavior.
```c
// Dangerous without parentheses:
SET_BIT(a + b, 2)  // Could expand incorrectly due to operator precedence
```
**Solution**: Wrapped all macro parameters in parentheses.

### Challenge 2: Multi-statement Macros
**Problem**: Creating macros with multiple statements that work in all contexts (if-else, loops).  
**Solution**: Used the `do-while(0)` idiom to ensure proper semicolon handling.


### Challenge 4: Testing Strategy
**Problem**: How to comprehensively test macros without a testing framework.  
**Solution**: Built a custom testing framework using macros, providing colored output and statistics.

### Challenge 5: Windows Development Environment
**Problem**: Windows doesn't have `make` by default. 
**Solution**: Provided multiple build options (Makefile, manual compilation, VS Code integration).

## 🔄 Future Enhancements

- Multibit manipulation Macros
- Test in an Embedded Environment


## 🤝 Contributing

While this is a learning project, feedback and suggestions are welcome! Please feel free to:
- Report bugs or issues
- Suggest improvements

## 📞 Contact

**Author**: [Melek GUEDIDI]  
**Email**: [melek.guedidi@insat.ucar.tn]  
**LinkedIn**: [https://www.linkedin.com/in/melek-guedidi/]  

---

**Part of the 30-Day Embedded C Challenge** | **Project 1/30** | **Next**: Q15.16 Fixed Point Math Library

*Building embedded systems expertise one project at a time* 🚀
