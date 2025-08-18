#include <stdio.h>
#include <assert.h>
#include "circular_buffer.h"

void test_create_destroy() {
    printf("Testing create/destroy...\n");
    
    // Test normal creation
    circular_buffer_t *cb = cb_create(10, sizeof(int));
    assert(cb != NULL);
    assert(cb->capacity == 10);
    assert(cb->element_size == sizeof(int));
    assert(cb->count == 0);
    assert(cb->head == 0);
    assert(cb->tail == 0);
    
    cb_destroy(cb);
    printf("✓ Normal create/destroy passed\n");
    
    // Test invalid parameters
    cb = cb_create(0, sizeof(int));
    assert(cb == NULL);
    printf("✓ Zero capacity rejected\n");
    
    cb = cb_create(10, 0);
    assert(cb == NULL);
    printf("✓ Zero element size rejected\n");
    
    // Test destroy with NULL (should not crash)
    cb_destroy(NULL);
    printf("✓ NULL destroy handled safely\n");
}

int main() {
    printf("=== Basic Circular Buffer Tests ===\n");
    test_create_destroy();
    printf("All tests passed!\n");
    return 0;
}