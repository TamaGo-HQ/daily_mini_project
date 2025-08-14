/**
 * @file test_bit_ops.c
 * @brief Comprehensive test suite for bit manipulation library
 * @author Your Name
 * @date 2025
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "../include/bit_ops.h"

/* Test result tracking */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color codes for output */
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

/* Test macros */
#define TEST_ASSERT(condition, test_name) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf(GREEN "✓ PASS: %s" RESET "\n", test_name); \
    } else { \
        tests_failed++; \
        printf(RED "✗ FAIL: %s" RESET "\n", test_name); \
    } \
} while(0)

#define TEST_SECTION(name) \
    printf(YELLOW "\n=== %s ===" RESET "\n", name)

/*============================================================================*
 *                              BASIC OPERATIONS TESTS                        *
 *============================================================================*/

void test_set_bit(void) {
    TEST_SECTION("SET_BIT Tests");
    
    /* Test with uint8_t */
    uint8_t val8 = 0x00;
    TEST_ASSERT(SET_BIT(val8, 0) == 0x01, "SET_BIT: Set bit 0 in uint8_t");
    TEST_ASSERT(SET_BIT(val8, 3) == 0x08, "SET_BIT: Set bit 3 in uint8_t");
    TEST_ASSERT(SET_BIT(val8, 7) == 0x80, "SET_BIT: Set bit 7 in uint8_t (MSB)");
    
    /* Test setting already set bit */
    val8 = 0xFF;
    TEST_ASSERT(SET_BIT(val8, 3) == 0xFF, "SET_BIT: Set already set bit");
    
    /* Test with uint16_t */
    uint16_t val16 = 0x0000;
    TEST_ASSERT(SET_BIT(val16, 8) == 0x0100, "SET_BIT: Set bit 8 in uint16_t");
    TEST_ASSERT(SET_BIT(val16, 15) == 0x8000, "SET_BIT: Set bit 15 in uint16_t (MSB)");
    
    /* Test with uint32_t */
    uint32_t val32 = 0x00000000;
    TEST_ASSERT(SET_BIT(val32, 16) == 0x00010000, "SET_BIT: Set bit 16 in uint32_t");
    TEST_ASSERT(SET_BIT(val32, 31) == 0x80000000, "SET_BIT: Set bit 31 in uint32_t (MSB)");
}

void test_clear_bit(void) {
    TEST_SECTION("CLEAR_BIT Tests");
    
    /* Test with uint8_t */
    uint8_t val8 = 0xFF;
    TEST_ASSERT(CLEAR_BIT(val8, 0) == 0xFE, "CLEAR_BIT: Clear bit 0 in uint8_t");
    TEST_ASSERT(CLEAR_BIT(val8, 3) == 0xF7, "CLEAR_BIT: Clear bit 3 in uint8_t");
    TEST_ASSERT(CLEAR_BIT(val8, 7) == 0x7F, "CLEAR_BIT: Clear bit 7 in uint8_t (MSB)");
    
    /* Test clearing already clear bit */
    val8 = 0x00;
    TEST_ASSERT(CLEAR_BIT(val8, 3) == 0x00, "CLEAR_BIT: Clear already clear bit");
    
    /* Test with uint16_t */
    uint16_t val16 = 0xFFFF;
    TEST_ASSERT(CLEAR_BIT(val16, 8) == 0xFEFF, "CLEAR_BIT: Clear bit 8 in uint16_t");
    TEST_ASSERT(CLEAR_BIT(val16, 15) == 0x7FFF, "CLEAR_BIT: Clear bit 15 in uint16_t (MSB)");
    
    /* Test with uint32_t */
    uint32_t val32 = 0xFFFFFFFF;
    TEST_ASSERT(CLEAR_BIT(val32, 16) == 0xFFFEFFFF, "CLEAR_BIT: Clear bit 16 in uint32_t");
    TEST_ASSERT(CLEAR_BIT(val32, 31) == 0x7FFFFFFF, "CLEAR_BIT: Clear bit 31 in uint32_t (MSB)");
}

void test_toggle_bit(void) {
    TEST_SECTION("TOGGLE_BIT Tests");
    
    /* Test with uint8_t */
    uint8_t val8 = 0x00;
    TEST_ASSERT(TOGGLE_BIT(val8, 0) == 0x01, "TOGGLE_BIT: Toggle bit 0 (0->1)");
    TEST_ASSERT(TOGGLE_BIT(val8, 3) == 0x08, "TOGGLE_BIT: Toggle bit 3 (0->1)");
    
    val8 = 0xFF;
    TEST_ASSERT(TOGGLE_BIT(val8, 0) == 0xFE, "TOGGLE_BIT: Toggle bit 0 (1->0)");
    TEST_ASSERT(TOGGLE_BIT(val8, 7) == 0x7F, "TOGGLE_BIT: Toggle bit 7 (1->0)");
    
    /* Test double toggle (should return to original) */
    val8 = 0xA5; /* 10100101 */
    uint8_t original = val8;
    val8 = TOGGLE_BIT(val8, 3);
    val8 = TOGGLE_BIT(val8, 3);
    TEST_ASSERT(val8 == original, "TOGGLE_BIT: Double toggle returns original");
    
    /* Test with uint16_t */
    uint16_t val16 = 0x5AA5;
    TEST_ASSERT(TOGGLE_BIT(val16, 8) == 0x5BA5, "TOGGLE_BIT: Toggle bit 8 in uint16_t");
    
    /* Test with uint32_t */
    uint32_t val32 = 0x12345678;
    TEST_ASSERT(TOGGLE_BIT(val32, 16) == 0x12355678, "TOGGLE_BIT: Toggle bit 16 in uint32_t");
}

void test_check_bit(void) {
    TEST_SECTION("CHECK_BIT Tests");
    
    /* Test with uint8_t */
    uint8_t val8 = 0xA5; /* 10100101 */
    TEST_ASSERT(CHECK_BIT(val8, 0) != 0, "CHECK_BIT: Check set bit 0");
    TEST_ASSERT(CHECK_BIT(val8, 1) == 0, "CHECK_BIT: Check clear bit 1");
    TEST_ASSERT(CHECK_BIT(val8, 2) != 0, "CHECK_BIT: Check set bit 2");
    TEST_ASSERT(CHECK_BIT(val8, 3) == 0, "CHECK_BIT: Check clear bit 3");
    TEST_ASSERT(CHECK_BIT(val8, 7) != 0, "CHECK_BIT: Check set bit 7 (MSB)");
    
    /* Test return values */
    TEST_ASSERT(CHECK_BIT(val8, 0) == 1, "CHECK_BIT: Returns correct value for bit 0");
    TEST_ASSERT(CHECK_BIT(val8, 2) == 4, "CHECK_BIT: Returns correct value for bit 2");
    TEST_ASSERT(CHECK_BIT(val8, 5) == 32, "CHECK_BIT: Returns correct value for bit 5");
    
    /* Test with all bits clear and all bits set */
    TEST_ASSERT(CHECK_BIT(0x00, 3) == 0, "CHECK_BIT: Check bit in all-clear value");
    TEST_ASSERT(CHECK_BIT(0xFF, 3) != 0, "CHECK_BIT: Check bit in all-set value");
    
    /* Test with uint16_t */
    uint16_t val16 = 0x8001;
    TEST_ASSERT(CHECK_BIT(val16, 0) != 0, "CHECK_BIT: Check bit 0 in uint16_t");
    TEST_ASSERT(CHECK_BIT(val16, 15) != 0, "CHECK_BIT: Check bit 15 in uint16_t");
    TEST_ASSERT(CHECK_BIT(val16, 8) == 0, "CHECK_BIT: Check clear bit in uint16_t");
}

void test_is_bit_set(void) {
    TEST_SECTION("IS_BIT_SET Tests");
    
    uint8_t val8 = 0xA5; /* 10100101 */
    TEST_ASSERT(IS_BIT_SET(val8, 0) == 1, "IS_BIT_SET: Returns 1 for set bit");
    TEST_ASSERT(IS_BIT_SET(val8, 1) == 0, "IS_BIT_SET: Returns 0 for clear bit");
    TEST_ASSERT(IS_BIT_SET(val8, 7) == 1, "IS_BIT_SET: Returns 1 for MSB");
    
    /* Verify it always returns 0 or 1 */
    for (int i = 0; i < 8; i++) {
        int result = IS_BIT_SET(val8, i);
        TEST_ASSERT(result == 0 || result == 1, 
                   "IS_BIT_SET: Always returns 0 or 1");
    }
}

/*============================================================================*
 *                              EDGE CASE TESTS                              *
 *============================================================================*/

void test_edge_cases(void) {
    TEST_SECTION("Edge Case Tests");
    
    /* Test with all zeros */
    uint8_t zero = 0x00;
    TEST_ASSERT(SET_BIT(zero, 0) == 0x01, "Edge: SET_BIT on all zeros");
    TEST_ASSERT(CLEAR_BIT(zero, 0) == 0x00, "Edge: CLEAR_BIT on all zeros");
    TEST_ASSERT(TOGGLE_BIT(zero, 0) == 0x01, "Edge: TOGGLE_BIT on all zeros");
    TEST_ASSERT(CHECK_BIT(zero, 0) == 0, "Edge: CHECK_BIT on all zeros");
    
    /* Test with all ones */
    uint8_t ones = 0xFF;
    TEST_ASSERT(SET_BIT(ones, 0) == 0xFF, "Edge: SET_BIT on all ones");
    TEST_ASSERT(CLEAR_BIT(ones, 0) == 0xFE, "Edge: CLEAR_BIT on all ones");
    TEST_ASSERT(TOGGLE_BIT(ones, 0) == 0xFE, "Edge: TOGGLE_BIT on all ones");
    TEST_ASSERT(CHECK_BIT(ones, 0) != 0, "Edge: CHECK_BIT on all ones");
    
    /* Test boundary bits */
    uint16_t val16 = 0x0000;
    TEST_ASSERT(SET_BIT(val16, 0) == 0x0001, "Edge: Set LSB in uint16_t");
    TEST_ASSERT(SET_BIT(val16, 15) == 0x8000, "Edge: Set MSB in uint16_t");
    
    uint32_t val32 = 0x00000000;
    TEST_ASSERT(SET_BIT(val32, 0) == 0x00000001, "Edge: Set LSB in uint32_t");
    TEST_ASSERT(SET_BIT(val32, 31) == 0x80000000, "Edge: Set MSB in uint32_t");
}

/*============================================================================*
 *                          REAL-WORLD SCENARIOS                             *
 *============================================================================*/

void test_embedded_scenarios(void) {
    TEST_SECTION("Embedded System Scenarios");
    
    /* GPIO Port simulation */
    uint8_t gpio_port = 0x00;
    
    /* Turn on LEDs on pins 2, 4, 6 */
    gpio_port = SET_BIT(gpio_port, 2);  // LED1
    gpio_port = SET_BIT(gpio_port, 4);  // LED2  
    gpio_port = SET_BIT(gpio_port, 6);  // LED3
    TEST_ASSERT(gpio_port == 0x54, "GPIO: Turn on multiple LEDs");
    
    /* Check if LED2 is on */
    TEST_ASSERT(CHECK_BIT(gpio_port, 4) != 0, "GPIO: Check LED status");
    
    /* Turn off LED1 */
    gpio_port = CLEAR_BIT(gpio_port, 2);
    TEST_ASSERT(gpio_port == 0x50, "GPIO: Turn off LED1");
    
    /* Toggle LED3 */
    gpio_port = TOGGLE_BIT(gpio_port, 6);
    TEST_ASSERT(gpio_port == 0x10, "GPIO: Toggle LED3 off");
    
    /* Status Register simulation */
    uint8_t status_reg = 0x00;
    
    /* Set various status bits */
    status_reg = SET_BIT(status_reg, 0);   // Ready bit
    status_reg = SET_BIT(status_reg, 3);   // Error bit
    status_reg = SET_BIT(status_reg, 7);   // Busy bit
    
    /* Check multiple conditions */
    int system_ready = IS_BIT_SET(status_reg, 0);
    int has_error = IS_BIT_SET(status_reg, 3);
    int is_busy = IS_BIT_SET(status_reg, 7);
    
    TEST_ASSERT(system_ready && has_error && is_busy, 
               "Status Register: Multiple bit checks");
    
    /* Clear error condition */
    status_reg = CLEAR_BIT(status_reg, 3);
    TEST_ASSERT(!IS_BIT_SET(status_reg, 3), "Status Register: Clear error bit");
}

/*============================================================================*
 *                            PERFORMANCE TESTS                              *
 *============================================================================*/

void test_macro_safety(void) {
    TEST_SECTION("Macro Safety Tests");
    
    /* Test with expressions (macro parameter safety) */
    uint8_t a = 5, b = 3;
    TEST_ASSERT(SET_BIT(a + b, 1) == 10, "Macro Safety: Expression parameters");
    
    /* Test multiple evaluations don't cause issues */
    uint8_t val = 8;
    uint8_t result = SET_BIT(val, 2);
    TEST_ASSERT(val == 8 && result == 12, 
               "Macro Safety: Original value unchanged");
    
    /* Test with different data types in same expression */
    uint16_t val16 = 0x1234;
    uint8_t pos = 8;
    TEST_ASSERT(SET_BIT(val16, pos) == 0x1334, 
               "Macro Safety: Mixed data types");
}

/*============================================================================*
 *                              TEST RUNNER                                  *
 *============================================================================*/

void print_test_summary(void) {
    printf(YELLOW "\n=== TEST SUMMARY ===" RESET "\n");
    printf("Total Tests: %d\n", tests_run);
    printf(GREEN "Passed: %d" RESET "\n", tests_passed);
    if (tests_failed > 0) {
        printf(RED "Failed: %d" RESET "\n", tests_failed);
    } else {
        printf("Failed: %d\n", tests_failed);
    }
    printf("Success Rate: %.1f%%\n", 
           tests_run > 0 ? (float)tests_passed / tests_run * 100.0 : 0.0);
}

int main(void) {
    printf(YELLOW "Starting Bit Operations Test Suite...\n" RESET);
    
    /* Run all test suites */
    test_set_bit();
    test_clear_bit();
    test_toggle_bit();
    test_check_bit();
    test_is_bit_set();
    test_edge_cases();
    test_embedded_scenarios();
    test_macro_safety();
    
    /* Print results */
    print_test_summary();
    
    /* Return appropriate exit code */
    return tests_failed > 0 ? 1 : 0;
}