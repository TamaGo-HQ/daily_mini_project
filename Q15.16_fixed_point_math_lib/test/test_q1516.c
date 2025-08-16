#include "q1516.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

/**
 * @file test_q1516.c
 * @brief Comprehensive test suite for Q15.16 Fixed-Point Library
 * @author tama_GO
 * @date Day 2 of 30-day Challenge
 */

// =============================================================================
// TEST UTILITIES
// =============================================================================

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TOLERANCE q1516_from_float(0.0001f)  // Small tolerance for floating-point comparisons

// Macro for test assertions
#define TEST_ASSERT(condition, test_name) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("✓ PASS: %s\n", test_name); \
    } else { \
        tests_failed++; \
        printf("✗ FAIL: %s\n", test_name); \
    } \
} while(0)

// Compare two fixed-point numbers with tolerance
bool fixed_point_equal(q1516_t a, q1516_t b, q1516_t tolerance) {
    return q1516_approximately_equal(a, b, tolerance);
}

// Print test section header
void print_section(const char* section_name) {
    printf("\n=== %s ===\n", section_name);
}

// =============================================================================
// CONVERSION TESTS
// =============================================================================

void test_conversions() {
    print_section("CONVERSION TESTS");
    
    // Integer conversions
    q1516_t fixed_1 = q1516_from_int(5);
    q1516_t fixed_2 = q1516_from_int(-3);
    q1516_t fixed_3 = q1516_from_int(0);
    
    TEST_ASSERT(q1516_to_int(fixed_1) == 5, "Integer conversion: 5");
    TEST_ASSERT(q1516_to_int(fixed_2) == -3, "Integer conversion: -3");
    TEST_ASSERT(q1516_to_int(fixed_3) == 0, "Integer conversion: 0");
    
    // Float conversions (basic)
    q1516_t fixed_pi = q1516_from_float(3.14159f);
    q1516_t fixed_neg = q1516_from_float(-2.5f);
    q1516_t fixed_small = q1516_from_float(0.125f);  // 1/8
    
    TEST_ASSERT(fabs(q1516_to_float(fixed_pi) - 3.14159f) < 0.0001f, "Float conversion: π");
    TEST_ASSERT(fabs(q1516_to_float(fixed_neg) - (-2.5f)) < 0.0001f, "Float conversion: -2.5");
    TEST_ASSERT(fabs(q1516_to_float(fixed_small) - 0.125f) < 0.0001f, "Float conversion: 0.125");
    
    // Edge cases
    q1516_t max_int = q1516_from_int(32767);  // Maximum integer that fits
    q1516_t min_int = q1516_from_int(-32768); // Minimum integer that fits
    
    TEST_ASSERT(q1516_to_int(max_int) == 32767, "Max integer conversion");
    TEST_ASSERT(q1516_to_int(min_int) == -32768, "Min integer conversion");
    
    printf("Conversion tests: %d/%d passed\n", 
           tests_passed - (tests_run - (tests_passed + tests_failed)), 
           tests_run - (tests_run - (tests_passed + tests_failed)));
}

// =============================================================================
// ARITHMETIC TESTS
// =============================================================================

void test_arithmetic() {
    print_section("ARITHMETIC TESTS");
    
    // Test data
    q1516_t a = q1516_from_float(5.75f);
    q1516_t b = q1516_from_float(2.25f);
    q1516_t c = q1516_from_float(-3.5f);
    
    // Addition tests
    q1516_t sum1 = q1516_add(a, b);  // 5.75 + 2.25 = 8.0
    q1516_t sum2 = q1516_add(a, c);  // 5.75 + (-3.5) = 2.25
    q1516_t sum3 = q1516_add(c, c);  // -3.5 + (-3.5) = -7.0
    
    TEST_ASSERT(fixed_point_equal(sum1, q1516_from_float(8.0f), TOLERANCE), "Addition: 5.75 + 2.25");
    TEST_ASSERT(fixed_point_equal(sum2, q1516_from_float(2.25f), TOLERANCE), "Addition: 5.75 + (-3.5)");
    TEST_ASSERT(fixed_point_equal(sum3, q1516_from_float(-7.0f), TOLERANCE), "Addition: (-3.5) + (-3.5)");
    
    // Subtraction tests
    q1516_t diff1 = q1516_subtract(a, b);  // 5.75 - 2.25 = 3.5
    q1516_t diff2 = q1516_subtract(b, a);  // 2.25 - 5.75 = -3.5
    q1516_t diff3 = q1516_subtract(a, c);  // 5.75 - (-3.5) = 9.25
    
    TEST_ASSERT(fixed_point_equal(diff1, q1516_from_float(3.5f), TOLERANCE), "Subtraction: 5.75 - 2.25");
    TEST_ASSERT(fixed_point_equal(diff2, q1516_from_float(-3.5f), TOLERANCE), "Subtraction: 2.25 - 5.75");
    TEST_ASSERT(fixed_point_equal(diff3, q1516_from_float(9.25f), TOLERANCE), "Subtraction: 5.75 - (-3.5)");
    
    // Multiplication tests
    q1516_t prod1 = q1516_multiply(a, b);  // 5.75 * 2.25 = 12.9375
    q1516_t prod2 = q1516_multiply(a, c);  // 5.75 * (-3.5) = -20.125
    q1516_t prod3 = q1516_multiply(c, c);  // (-3.5) * (-3.5) = 12.25
    
    TEST_ASSERT(fixed_point_equal(prod1, q1516_from_float(12.9375f), TOLERANCE), "Multiplication: 5.75 * 2.25");
    TEST_ASSERT(fixed_point_equal(prod2, q1516_from_float(-20.125f), TOLERANCE), "Multiplication: 5.75 * (-3.5)");
    TEST_ASSERT(fixed_point_equal(prod3, q1516_from_float(12.25f), TOLERANCE), "Multiplication: (-3.5) * (-3.5)");
    
    // Division tests
    q1516_t quot1 = q1516_divide(a, b);   // 5.75 / 2.25 = 2.555...
    q1516_t quot2 = q1516_divide(b, a);   // 2.25 / 5.75 = 0.391...
    q1516_t quot3 = q1516_divide(a, c);   // 5.75 / (-3.5) = -1.642...
    
    TEST_ASSERT(fabs(q1516_to_float(quot1) - (5.75f/2.25f)) < 0.001f, "Division: 5.75 / 2.25");
    TEST_ASSERT(fabs(q1516_to_float(quot2) - (2.25f/5.75f)) < 0.001f, "Division: 2.25 / 5.75");
    TEST_ASSERT(fabs(q1516_to_float(quot3) - (5.75f/-3.5f)) < 0.001f, "Division: 5.75 / (-3.5)");
    
    // Special cases
    q1516_t zero = q1516_from_int(0);
    q1516_t one = q1516_from_int(1);
    
    TEST_ASSERT(q1516_add(a, zero) == a, "Addition identity: a + 0 = a");
    TEST_ASSERT(q1516_multiply(a, one) == a, "Multiplication identity: a * 1 = a");
    TEST_ASSERT(q1516_divide(a, one) == a, "Division identity: a / 1 = a");
}

// =============================================================================
// UTILITY FUNCTION TESTS
// =============================================================================

void test_utilities() {
    print_section("UTILITY FUNCTION TESTS");
    
    // Absolute value tests
    q1516_t pos = q1516_from_float(3.5f);
    q1516_t neg = q1516_from_float(-3.5f);
    q1516_t zero = q1516_from_int(0);
    
    TEST_ASSERT(q1516_abs(pos) == pos, "Absolute value: |3.5| = 3.5");
    TEST_ASSERT(q1516_abs(neg) == pos, "Absolute value: |-3.5| = 3.5");
    TEST_ASSERT(q1516_abs(zero) == zero, "Absolute value: |0| = 0");
    
    // Integer part extraction
    q1516_t mixed1 = q1516_from_float(7.25f);
    q1516_t mixed2 = q1516_from_float(-5.75f);
    
    TEST_ASSERT(q1516_get_integer_part(mixed1) == 7, "Integer part: 7.25 → 7");
    TEST_ASSERT(q1516_get_integer_part(mixed2) == -5, "Integer part: -5.75 → -5");
    
    // Fractional part extraction
    q1516_t frac1 = q1516_get_fractional_part(mixed1);  // Should be 0.25
    q1516_t frac2 = q1516_get_fractional_part(mixed2);  // Should be 0.75 (positive!)
    
    TEST_ASSERT(fixed_point_equal(frac1, q1516_from_float(0.25f), TOLERANCE), "Fractional part: 7.25 → 0.25");
    TEST_ASSERT(fixed_point_equal(frac2, q1516_from_float(0.75f), TOLERANCE), "Fractional part: -5.75 → 0.75");
    // Note: Fractional part of negative numbers is still positive in our implementation
    
    // Approximate equality tests
    q1516_t val1 = q1516_from_float(3.14159f);
    q1516_t val2 = q1516_from_float(3.14160f);  // Very close
    q1516_t val3 = q1516_from_float(3.15f);     // Further away
    
    q1516_t small_tolerance = q1516_from_float(0.001f);
    q1516_t large_tolerance = q1516_from_float(0.01f);
    
    TEST_ASSERT(q1516_approximately_equal(val1, val2, small_tolerance), "Approximate equality: close values");
    TEST_ASSERT(!q1516_approximately_equal(val1, val3, small_tolerance), "Approximate equality: distant values (small tolerance)");
    TEST_ASSERT(q1516_approximately_equal(val1, val3, large_tolerance), "Approximate equality: distant values (large tolerance)");
}

// =============================================================================
// EDGE CASE TESTS
// =============================================================================

void test_edge_cases() {
    print_section("EDGE CASE TESTS");
    
    // Division by zero
    q1516_t dividend = q1516_from_float(5.0f);
    q1516_t zero = q1516_from_int(0);
    q1516_t result = q1516_divide(dividend, zero);
    
    TEST_ASSERT(result == Q1516_MAX, "Division by zero: positive dividend");
    
    q1516_t neg_dividend = q1516_from_float(-5.0f);
    q1516_t neg_result = q1516_divide(neg_dividend, zero);
    
    TEST_ASSERT(neg_result == Q1516_MIN, "Division by zero: negative dividend");
    
    // Very small numbers
    q1516_t very_small = q1516_from_float(0.00002f);  // Close to precision limit
    TEST_ASSERT(q1516_to_float(very_small) > 0.0f, "Very small positive number preservation");
    
    // Large numbers (within range)
    q1516_t large = q1516_from_float(30000.0f);
    TEST_ASSERT(fabs(q1516_to_float(large) - 30000.0f) < 1.0f, "Large number conversion");
    
    // Precision test
    q1516_t eighth = q1516_from_float(0.125f);  // 1/8 - exactly representable
    TEST_ASSERT(q1516_to_float(eighth) == 0.125f, "Exact fractional representation");
}

// =============================================================================
// PERFORMANCE DEMONSTRATION
// =============================================================================

void test_performance() {
    print_section("PERFORMANCE DEMONSTRATION");
    
    printf("Performing 1,000,000 operations...\n");
    
    q1516_t a = q1516_from_float(3.14159f);
    q1516_t b = q1516_from_float(2.71828f);
    q1516_t result = 0;
    
    // Simple performance test (not precise timing, but shows it works)
    for (int i = 0; i < 1000000; i++) {
        result = q1516_multiply(a, b);
        result = q1516_add(result, q1516_from_int(1));
        result = q1516_divide(result, q1516_from_int(2));
    }
    
    printf("Final result: %.6f\n", q1516_to_float(result));
    printf("Performance test completed successfully!\n");
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrate_library() {
    print_section("LIBRARY DEMONSTRATION");
    
    printf("Creating some fixed-point numbers:\n");
    q1516_t pi = q1516_from_float(3.14159f);
    q1516_t e = q1516_from_float(2.71828f);
    q1516_t golden_ratio = q1516_from_float(1.618f);
    
    q1516_print("π (pi)", pi);
    q1516_print("e (Euler's number)", e);
    q1516_print("φ (golden ratio)", golden_ratio);
    
    printf("\nArithmetic operations:\n");
    q1516_t sum = q1516_add(pi, e);
    q1516_t product = q1516_multiply(pi, e);
    q1516_t quotient = q1516_divide(pi, e);
    
    q1516_print("π + e", sum);
    q1516_print("π × e", product);
    q1516_print("π ÷ e", quotient);
    
    printf("\nDetailed breakdown of π:\n");
    q1516_print_detailed("π (pi)", pi);
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main() {
    printf("Q15.16 Fixed-Point Library - Comprehensive Test Suite\n");
    printf("=====================================================\n");
    
    // Run all test suites
    test_conversions();
    test_arithmetic();
    test_utilities();
    test_edge_cases();
    test_performance();
    demonstrate_library();
    
    // Final results
    print_section("TEST RESULTS");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
    if (tests_failed == 0) {
        printf("\n🎉 ALL TESTS PASSED! 🎉\n");
        printf("Your Q15.16 library is working correctly!\n");
        return 0;
    } else {
        printf("\n❌ %d TESTS FAILED ❌\n", tests_failed);
        printf("Please review the failed tests above.\n");
        return 1;
    }
}

/*
 * WHAT THIS TEST SUITE COVERS:
 * 
 * 1. CONVERSION TESTING:
 *    - Integer ↔ Fixed-point conversions
 *    - Float ↔ Fixed-point conversions  
 *    - Edge cases (max/min values)
 * 
 * 2. ARITHMETIC TESTING:
 *    - All four basic operations (+, -, ×, ÷)
 *    - Positive, negative, and mixed sign operations
 *    - Identity operations (a+0, a×1, a÷1)
 * 
 * 3. UTILITY TESTING:
 *    - Absolute value function
 *    - Integer/fractional part extraction
 *    - Approximate equality comparison
 * 
 * 4. EDGE CASE TESTING:
 *    - Division by zero handling
 *    - Very small numbers (precision limits)
 *    - Large numbers (range limits)
 *    - Exact fractional representations
 * 
 * 5. PERFORMANCE TESTING:
 *    - Million-operation benchmark
 *    - Demonstrates speed of fixed-point math
 * 
 * 6. DEMONSTRATION:
 *    - Shows library usage with real constants
 *    - Pretty-printed output examples
 */