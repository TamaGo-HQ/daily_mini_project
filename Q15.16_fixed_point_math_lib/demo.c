#include "q1516.h" 
#include <stdio.h> 
 
int main() { 
    printf("Q15.16 Fixed-Point Library Demo\n"); 
    printf("================================\n\n"); 
 
    // Create some fixed-point numbers 
    q1516_t a = q1516_from_float(3.14159f); 
    q1516_t b = q1516_from_float(2.0f); 
    q1516_t c = q1516_from_int(7); 
 
    // Perform operations 
    q1516_t sum = q1516_add(a, b); 
    q1516_t product = q1516_multiply(a, b); 
    q1516_t quotient = q1516_divide(c, b); 
 
    // Display results 
    q1516_print("π (pi)", a); 
    q1516_print("2.0", b); 
    q1516_print("7", c); 
    printf("\nArithmetic operations:\n"); 
    q1516_print("π + 2.0", sum); 
    q1516_print("π × 2.0", product); 
    q1516_print("7 ÷ 2.0", quotient); 
 
    printf("\nDetailed view of π:\n"); 
    q1516_print_detailed("π (pi)", a); 
 
    return 0; 
} 
