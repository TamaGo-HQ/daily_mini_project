#ifndef Q1516_H
#define Q1516_H

#include <stdint.h>
#include <stdbool.h>

/* 
 *   @file q1516.h
 *   @brief Q15.16 Fixed-Point Arithmetic Library
 *   @author tama_GO
 *   @date Day 2 of 30-day Challenge
 *
 *   This Library implements Q15.16 fixed-point arithmetic for embedded systems.
 *   Q15.16 format uses 32 bits: 1 sign + 15 integer + 16 fractional bits
 *
 * Range:    [-32768.0, 32767.9999847412]
 * Quantum:  1/65536 = 1.5259e-5
 */

// =============================================================================
// C++ COMPATIBILITY BLOCK
// =============================================================================
// This makes the library work with both C and C++ compilers
#ifdef __cplusplus  // Defined automatically by C++ compilers
extern "C" {        // Tells C++: "don't mangle these function names"
#endif

// =============================================================================
// TYPE DEFINITIONS
// =============================================================================

// Create a new type name for clarity and maintainability
typedef int32_t q1516_t;  // "q1516_t" is more descriptive than "int32_t"
// =============================================================================
// CONSTANTS
// =============================================================================

// Use 1L (long) to ensure 32-bit arithmetic even on 16-bit systems
#define Q1516_FRACTIONAL_BITS 16
#define Q1516_SCALE (1L << Q1516_FRACTIONAL_BITS)  // 65536

// Useful constants in Q15.16 format
#define Q1516_ONE Q1516_SCALE           // 1.0 in Q15.16
#define Q1516_HALF (Q1516_SCALE >> 1)   // 0.5 in Q15.16

// Maximum and minimum values for overflow handling
#define Q1516_MAX ((q1516_t)0x7FFFFFFF)
#define Q1516_MIN ((q1516_t)0x80000000)
// =============================================================================
// FUNCTION DECLARATIONS (not macros!)
// =============================================================================

// Why functions instead of macros:
// 1. Type safety - compiler checks parameter types
// 2. Single evaluation - no side effects from multiple macro expansion
// 3. Debuggable - you can step through function calls
// 4. Cleaner error messages

/*
 * @brief Convert integer to Q15.16 fixed-point
 * @param value Integer value to convert
 * @return Q15.16 fixed-point representation
 */
q1516_t q1516_from_int(int32_t value);

/*
 * @brief Convert Q15.16 fixed-point to integer (truncated)
 * @param fixed Q15.16 fixed-point value
 * @return Integer part (truncated)
 */
int32_t q1516_to_int(q1516_t fixed);

/*
 * @brief Convert float to Q15.16 fixed-point
 * @param value Float value to convert
 * @return Q15.16 fixed-point representation
 */
q1516_t q1516_from_float(float value);

/*
 * @brief Convert Q15.16 fixed-point to float
 * @param fixed Q15.16 fixed-point value
 * @return Float representation
 */
float q1516_to_float(q1516_t fixed);

// =============================================================================
// ARITHMETIC FUNCTIONS - DECLARATIONS ONLY
// =============================================================================

/*
 * @brief Add two Q15.16 fixed-point numbers
 * @param a First operand
 * @param b Second operand
 * @return Sum of a and b
 */
q1516_t q1516_add(q1516_t a, q1516_t b);

/*
 * @brief Subtract two Q15.16 fixed-point numbers
 * @param a First operand (minuend)
 * @param b Second operand (subtrahend)
 * @return Difference (a - b)
 */
q1516_t q1516_subtract(q1516_t a, q1516_t b);

/*
 * @brief Multiply two Q15.16 fixed-point numbers
 * @param a First operand
 * @param b Second operand
 * @return Product of a and b
 */
q1516_t q1516_multiply(q1516_t a, q1516_t b);

/*
 * @brief Divide two Q15.16 fixed-point numbers
 * @param dividend Dividend (numerator)
 * @param divisor Divisor (denominator)
 * @return Quotient (dividend / divisor)
 */
q1516_t q1516_divide(q1516_t dividend, q1516_t divisor);

// =============================================================================
// UTILITY FUNCTIONS - DECLARATIONS ONLY
// =============================================================================

/*
 * @brief Get absolute value of Q15.16 fixed-point number
 * @param value Input value
 * @return Absolute value
 */
q1516_t q1516_abs(q1516_t value);

/*
 * @brief Get the integer part of a Q15.16 fixed-point number
 * @param fixed Q15.16 fixed-point value
 * @return Integer part only
 */
int32_t q1516_get_integer_part(q1516_t fixed);

/*
 * @brief Get the fractional part of a Q15.16 fixed-point number
 * @param fixed Q15.16 fixed-point value
 * @return Fractional part as Q15.16 format
 */
q1516_t q1516_get_fractional_part(q1516_t fixed);

/*
 * @brief Check if two Q15.16 numbers are approximately equal
 * @param a First number
 * @param b Second number
 * @param tolerance Tolerance for comparison (in Q15.16 format)
 * @return true if |a - b| <= tolerance
 */
bool q1516_approximately_equal(q1516_t a, q1516_t b, q1516_t tolerance);

// =============================================================================
// DEBUGGING FUNCTIONS - DECLARATIONS ONLY
// =============================================================================

/*
 * @brief Print Q15.16 fixed-point number in human-readable format
 * @param label Description label
 * @param fixed Q15.16 fixed-point value
 */
void q1516_print(const char* label, q1516_t fixed);

/*
 * @brief Print detailed breakdown of Q15.16 representation
 * @param label Description label
 * @param fixed Q15.16 fixed-point value
 */
void q1516_print_detailed(const char* label, q1516_t fixed);

// Close the C++ compatibility block
#ifdef __cplusplus
}
#endif

#endif /* Q1516_H */

/*
 * COMPARISON: Macros vs Functions
 * 
 * MACRO APPROACH (your original):
 * #define ADD_FIXED(a,b) ((a) + (b))
 * 
 * Problems:
 * - ADD_FIXED(func1(), func2()) calls functions TWICE
 * - No type checking: ADD_FIXED("hello", 42) compiles
 * - Hard to debug - debugger shows expanded macro text
 * - No scope - can conflict with other names
 * 
 * FUNCTION APPROACH (what we're using):
 * q1516_t q1516_add(q1516_t a, q1516_t b);
 * 
 * Benefits:
 * - Arguments evaluated exactly once
 * - Type checking: q1516_add("hello", 42) gives clear error
 * - Can set breakpoints and step through
 * - Clean function names in debugger
 * - Proper scoping rules
 */