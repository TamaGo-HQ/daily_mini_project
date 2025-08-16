#include "q1516.h"
#include <stdio.h>

/*
 *@file q1516.c
 * @brief Implementation of Q15.16 Fixed-Point Arithmetic Library
 */

//=========================================
// CONVERSION FUNCTIONS
//=========================================

q1516_t q1516_from_int(int32_t value){
    return (q1516_t)(value << Q1516_FRACTIONAL_BITS);
}

int32_t q1516_to_int(q1516_t fixed){
    return (int32_t)(fixed >> Q1516_FRACTIONAL_BITS);
}

q1516_t q1516_from_float(float value){
    return (q1516_t)(value * Q1516_SCALE);
}

float q1516_to_float(q1516_t fixed){
    // CORRECTION: original was: return fixed / Q1516_SCALE;
    // PROBLEM: Integer division! This truncates the result
    // BETTER: Cast to float first to get proper division
    return (float)fixed / Q1516_SCALE;
}

//=========================================
// Arithmetic FUNCTIONS
//=========================================

q1516_t q1516_add(q1516_t a, q1516_t b){
    return a + b;
}

q1516_t q1516_subtract(q1516_t a, q1516_t b){
    return a - b;
}

q1516_t q1516_multiply(q1516_t a, q1516_t b){
    int64_t temp = (int64_t)a * (int64_t)b;
    return (q1516_t)(temp >> Q1516_FRACTIONAL_BITS);
}

q1516_t q1516_divide(q1516_t dividend, q1516_t divisor){
    if (divisor == 0){
        return (dividend >= 0) ? Q1516_MAX : Q1516_MIN;
    }
    int64_t temp = ((int64_t)dividend << Q1516_FRACTIONAL_BITS) / divisor;
    return (q1516_t)temp;
}

//=========================================
// Utility FUNCTIONS
//=========================================

q1516_t q1516_abs(q1516_t fixed){
    if(fixed == Q1516_MIN){
        return Q1516_MAX;
    }
    return (fixed > 0) ? fixed : -fixed;
}

q1516_t q1516_get_fractional_part(q1516_t fixed){
    if(fixed < 0) {fixed = -fixed;}
    return fixed & ((1L << Q1516_FRACTIONAL_BITS) - 1);
}

int32_t q1516_get_integer_part(q1516_t fixed){
    int32_t result = fixed >> Q1516_FRACTIONAL_BITS;
    if (fixed < 0 && (fixed & ((1 << Q1516_FRACTIONAL_BITS) - 1))) {
        // If negative and had fractional part, add 1 to cancel floor rounding
        result += 1;
    }
    return result;
}

bool q1516_approximately_equal(q1516_t a, q1516_t b, q1516_t tolerance){
    q1516_t difference = q1516_abs(a - b);
    return difference <= tolerance;
}

//=========================================
// Debugging FUNCTIONS
//=========================================

void q1516_print(const char* label, q1516_t fixed){
    printf("%s: %.6f (raw: %d)\n", label, q1516_to_float(fixed), fixed);
}

void q1516_print_detailed(const char* label, q1516_t fixed){
    int32_t int_part = q1516_get_integer_part(fixed);
    q1516_t frac_part = q1516_get_fractional_part(fixed);
    
    printf("%s:\n", label);
    printf("  Raw value: %d (0x%08X)\n", fixed, (uint32_t)fixed);
    printf("  As float: %.6f\n", q1516_to_float(fixed));
    printf("  Integer part: %d\n", int_part);
    printf("  Fractional part: %d (%.6f)\n", frac_part, q1516_to_float(frac_part));
}