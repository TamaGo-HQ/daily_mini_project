# Q15.16 Fixed-Point Arithmetic Library for Embedded Systems

**Project 2 of 30** - *30 Days of C Programming for Embedded Systems*

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A fixed-point arithmetic library implementing Q15.16 format for systems where floating-point operations are expensive or unavailable.

## 🎯 Motivation

Embedded systems often lack dedicated floating-point units (FPUs), making floating-point operations slow and resource-intensive. Many microcontrollers either don't support hardware floating-point at all, or the operations consume significant CPU cycles and power.

**The Problem:**
- Floating-point math is expensive on embedded systems
- Some microcontrollers lack FPU entirely
- Need for fractional precision without floating-point overhead
- Real-time systems require predictable, fast arithmetic operations

**The Solution:**
Fixed-point arithmetic provides a perfect middle ground - offering fractional precision with the speed of integer operations. The Q15.16 format uses 32 bits to represent numbers with 15 integer bits and 16 fractional bits, giving us:
- **Range:** -32,768.0 to 32,767.9999847412
- **Precision:** 1/65536 ≈ 0.000015
- **Speed:** Integer-level performance with fractional capability

## 📊 Features

### Core Arithmetic Operations
- **Addition & Subtraction**
- **Multiplication** 
- **Division** 

### Conversion Functions
- **Integer ↔ Fixed-point** 
- **Float ↔ Fixed-point** 

### Utility Functions
- **Absolute value**
- **Part extraction:** Get integer and fractional parts separately
- **Approximate equality:** Tolerance-based comparison
- **Debug printing:** Human-readable output with detailed breakdowns

### Safety Features
- **Overflow protection:** Saturates to max/min values instead of wrapping
- **Divide-by-zero handling:** Returns appropriate infinity values
- **Edge case management:** Proper handling of boundary conditions

## 🚀 Getting Started

### Prerequisites
- C compiler
- Make (for using the Makefile)

### Quick Start

1. **Clone the repository:**
   ```bash
   git clone <your-repo-url>
   cd q1516-fixed-point
   ```

2. **Compile and run the demo:**
   ```bash
   make demo
   ./demo
   ```

3. **Run the comprehensive test suite:**
   ```bash
   make test
   ./test_q1516
   ```

4. **Clean build artifacts:**
   ```bash
   make clean
   ```

## 📖 API Reference

### Data Types
```c
typedef int32_t q1516_t;  // Q15.16 fixed-point type
```

### Constants
```c
#define Q1516_FRACTIONAL_BITS 16
#define Q1516_SCALE (1L << Q1516_FRACTIONAL_BITS)  // 65536
#define Q1516_ONE Q1516_SCALE                      // 1.0 in Q15.16
#define Q1516_HALF (Q1516_SCALE >> 1)              // 0.5 in Q15.16
#define Q1516_MAX ((q1516_t)0x7FFFFFFF)            // Maximum value
#define Q1516_MIN ((q1516_t)0x80000000)            // Minimum value
```

### Conversion Functions
```c
q1516_t q1516_from_int(int32_t value);    // Convert integer to Q15.16
int32_t q1516_to_int(q1516_t fixed);      // Convert Q15.16 to integer (truncated)
q1516_t q1516_from_float(float value);    // Convert float to Q15.16
float q1516_to_float(q1516_t fixed);      // Convert Q15.16 to float
```

### Arithmetic Functions
```c
q1516_t q1516_add(q1516_t a, q1516_t b);           // Addition
q1516_t q1516_subtract(q1516_t a, q1516_t b);      // Subtraction
q1516_t q1516_multiply(q1516_t a, q1516_t b);      // Multiplication
q1516_t q1516_divide(q1516_t dividend, q1516_t divisor);  // Division
```

### Utility Functions
```c
q1516_t q1516_abs(q1516_t value);                          // Absolute value
int32_t q1516_get_integer_part(q1516_t fixed);             // Get integer part
q1516_t q1516_get_fractional_part(q1516_t fixed);          // Get fractional part
bool q1516_approximately_equal(q1516_t a, q1516_t b, q1516_t tolerance);  // Fuzzy comparison
```

### Debug Functions
```c
void q1516_print(const char* label, q1516_t fixed);         // Simple print
void q1516_print_detailed(const char* label, q1516_t fixed); // Detailed breakdown
```

## 🧪 Test Suite

The comprehensive test suite (`test_q1516.c`) covers:

- **Conversion Tests:** Integer/float conversions and edge cases
- **Arithmetic Tests:** All operations with positive, negative, and mixed values
- **Utility Tests:** Helper functions and their edge cases
- **Edge Case Tests:** Division by zero, overflow handling, precision limits

### Expected Output
```
Q15.16 Fixed-Point Library - Comprehensive Test Suite
=====================================================

=== CONVERSION TESTS ===
✓ PASS: Integer conversion: 5
✓ PASS: Integer conversion: -3
... (all tests)

=== TEST RESULTS ===
Tests run: 42
Tests passed: 42
Tests failed: 0

 ALL TESTS PASSED! 
Your Q15.16 library is working correctly!
```

## 🔧 Technical Challenges & Solutions

### Challenge 1: Division Precision Loss
**Problem:** Integer division truncates results, losing precision.

**Solution:** Left-shift dividend by 16 bits before division:
```c
q1516_t q1516_divide(q1516_t dividend, q1516_t divisor) {
    if (divisor == 0) {
        return (dividend >= 0) ? Q1516_MAX : Q1516_MIN;
    }
    int64_t temp = ((int64_t)dividend << Q1516_FRACTIONAL_BITS) / divisor;
    return (q1516_t)temp;
}
```

### Challenge 2: Negative Number Handling
**Problem:** Right-shifting negative numbers performs arithmetic shift, not logical shift.

**Solution:** Careful handling of sign extension and fractional part extraction:
```c
int32_t q1516_get_integer_part(q1516_t fixed) {
    int32_t result = fixed >> Q1516_FRACTIONAL_BITS;
    if (fixed < 0 && (fixed & ((1 << Q1516_FRACTIONAL_BITS) - 1))) {
        result += 1;  // Compensate for floor rounding
    }
    return result;
}
```

### Challenge 3: Float Conversion Accuracy
**Problem:** Integer division in float conversion truncated results.

**Solution:** Cast to float before division:
```c
float q1516_to_float(q1516_t fixed) {
    return (float)fixed / Q1516_SCALE;  // Not: fixed / Q1516_SCALE
}
```

### Challenge 5: Function vs. Macro Decision
**Problem:** Macros lack type safety.

**Solution:** Used functions instead of macros for:
- Type safety 
- Better maintainability

## 📈 Performance Characteristics

- **Addition/Subtraction:** Single CPU instruction (fastest possible)
- **Multiplication:** ~3-5 CPU instructions (64-bit intermediate)
- **Division:** ~10-15 CPU instructions (still much faster than float)
- **Conversions:** 1-2 CPU instructions (bit shifting/multiplication)


## 🔮 Future Enhancements

- [ ] **Math Functions:** sin, cos, sqrt using lookup tables or Taylor series
- [ ] **Different Formats:** Q8.24, Q31.32 variants

## 🤝 Contributing

While this is a learning project, feedback and suggestions are welcome! Please feel free to:
- Report bugs or issues
- Suggest improvements

## 📞 Contact

**Author**: [Melek GUEDIDI]  
**Email**: [melek.guedidi@insat.ucar.tn]  
**LinkedIn**: [https://www.linkedin.com/in/melek-guedidi/]  

---

**Part of the 30-Day Embedded C Challenge** | **Project 2/30** | **Next**: Circular Buffer Library for Embedded Systems

*Building embedded systems expertise one project at a time* 🚀
