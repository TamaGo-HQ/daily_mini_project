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

void test_status_functions() {
    printf("Testing status functions...\n");
    
    circular_buffer_t *cb = cb_create(5, sizeof(int));
    assert(cb != NULL);
    
    // Test initial state
    assert(cb_is_empty(cb) == true);
    assert(cb_is_full(cb) == false);
    assert(cb_count(cb) == 0);
    assert(cb_capacity(cb) == 5);
    assert(cb_free_space(cb) == 5);
    printf("✓ Initial state correct\n");
    
    // Test NULL handling
    assert(cb_is_empty(NULL) == true);
    assert(cb_is_full(NULL) == false);
    assert(cb_count(NULL) == 0);
    assert(cb_capacity(NULL) == 0);
    assert(cb_free_space(NULL) == 0);
    printf("✓ NULL handling correct\n");
    
    cb_destroy(cb);
}

void test_put_get_basic() {
    printf("Testing basic put/get operations...\n");
    
    circular_buffer_t *cb = cb_create(3, sizeof(int));
    assert(cb != NULL);
    
    // Test putting data
    int test_data[] = {10, 20, 30, 40, 50};
    int read_data;
    
    // Put first element
    assert(cb_put(cb, &test_data[0]) == CB_SUCCESS);
    assert(cb_count(cb) == 1);
    assert(!cb_is_empty(cb));
    assert(!cb_is_full(cb));
    
    // Put second element
    assert(cb_put(cb, &test_data[1]) == CB_SUCCESS);
    assert(cb_count(cb) == 2);
    
    // Put third element (buffer should be full now)
    assert(cb_put(cb, &test_data[2]) == CB_SUCCESS);
    assert(cb_count(cb) == 3);
    assert(cb_is_full(cb));
    
    // Try to put when full (should fail)
    assert(cb_put(cb, &test_data[3]) == CB_ERROR_FULL);
    assert(cb_count(cb) == 3); // Count shouldn't change
    
    printf("✓ Put operations work correctly\n");
    
    // Test getting data (should come out in FIFO order)
    assert(cb_get(cb, &read_data) == CB_SUCCESS);
    assert(read_data == 10); // First in, first out
    assert(cb_count(cb) == 2);
    
    assert(cb_get(cb, &read_data) == CB_SUCCESS);
    assert(read_data == 20);
    assert(cb_count(cb) == 1);
    
    assert(cb_get(cb, &read_data) == CB_SUCCESS);
    assert(read_data == 30);
    assert(cb_count(cb) == 0);
    assert(cb_is_empty(cb));
    
    // Try to get when empty (should fail)
    assert(cb_get(cb, &read_data) == CB_ERROR_EMPTY);
    
    printf("✓ Get operations work correctly\n");
    
    cb_destroy(cb);
}
void test_circular_behavior() {
    printf("Testing circular wrap-around...\n");
    
    circular_buffer_t *cb = cb_create(3, sizeof(int));
    int data, result;
    
    // Fill buffer
    data = 1; cb_put(cb, &data);
    data = 2; cb_put(cb, &data);
    data = 3; cb_put(cb, &data);
    
    // Remove one element
    cb_get(cb, &result);
    assert(result == 1);
    
    // Add another (this should wrap around)
    data = 4;
    assert(cb_put(cb, &data) == CB_SUCCESS);
    
    // Verify the remaining order
    cb_get(cb, &result); assert(result == 2);
    cb_get(cb, &result); assert(result == 3);
    cb_get(cb, &result); assert(result == 4);
    
    printf("✓ Circular wrap-around works\n");
    
    cb_destroy(cb);
}

void test_null_pointers() {
    printf("Testing NULL pointer handling...\n");
    
    circular_buffer_t *cb = cb_create(5, sizeof(int));
    int data = 42;
    
    // Test NULL buffer
    assert(cb_put(NULL, &data) == CB_ERROR_NULL_POINTER);
    assert(cb_get(NULL, &data) == CB_ERROR_NULL_POINTER);
    
    // Test NULL data
    assert(cb_put(cb, NULL) == CB_ERROR_NULL_POINTER);
    assert(cb_get(cb, NULL) == CB_ERROR_NULL_POINTER);
    
    printf("✓ NULL pointer handling works\n");
    
    cb_destroy(cb);
}
void test_peek() {
    printf("Testing peek operation...\n");
    
    circular_buffer_t *cb = cb_create(3, sizeof(int));
    int data, peek_data;
    
    // Peek on empty buffer should fail
    assert(cb_peek(cb, &peek_data) == CB_ERROR_EMPTY);
    
    // Add some data
    data = 100; cb_put(cb, &data);
    data = 200; cb_put(cb, &data);
    
    // Peek should return first element without removing it
    assert(cb_peek(cb, &peek_data) == CB_SUCCESS);
    assert(peek_data == 100);
    assert(cb_count(cb) == 2); // Count unchanged
    
    // Peek again - should get same data
    assert(cb_peek(cb, &peek_data) == CB_SUCCESS);
    assert(peek_data == 100);
    
    // Now get - should get same data but remove it
    assert(cb_get(cb, &data) == CB_SUCCESS);
    assert(data == 100);
    assert(cb_count(cb) == 1);
    
    // Peek should now show next element
    assert(cb_peek(cb, &peek_data) == CB_SUCCESS);
    assert(peek_data == 200);
    
    printf("✓ Peek works correctly\n");
    cb_destroy(cb);
}

void test_reset() {
    printf("Testing reset operation...\n");
    
    circular_buffer_t *cb = cb_create(5, sizeof(int));
    int data;
    
    // Fill buffer partially
    for (int i = 0; i < 3; i++) {
        data = i * 10;
        cb_put(cb, &data);
    }
    
    assert(cb_count(cb) == 3);
    assert(!cb_is_empty(cb));
    
    // Reset should empty the buffer
    cb_reset(cb);
    assert(cb_count(cb) == 0);
    assert(cb_is_empty(cb));
    assert(!cb_is_full(cb));
    assert(cb->head == 0);
    assert(cb->tail == 0);
    
    // Should be able to use buffer normally after reset
    data = 999;
    assert(cb_put(cb, &data) == CB_SUCCESS);
    assert(cb_get(cb, &data) == CB_SUCCESS);
    assert(data == 999);
    
    printf("✓ Reset works correctly\n");
    cb_destroy(cb);
}

void test_different_data_types() {
    printf("Testing different data types...\n");
    
    // Test with chars
    circular_buffer_t *char_cb = cb_create(5, sizeof(char));
    char c_in = 'A', c_out;
    
    cb_put(char_cb, &c_in);
    cb_get(char_cb, &c_out);
    assert(c_out == 'A');
    
    cb_destroy(char_cb);
    
    // Test with floats
    circular_buffer_t *float_cb = cb_create(3, sizeof(float));
    float f_in = 3.14f, f_out;
    
    cb_put(float_cb, &f_in);
    cb_get(float_cb, &f_out);
    assert(f_out == 3.14f);
    
    cb_destroy(float_cb);
    
    printf("✓ Different data types work\n");
}

int main() {
    // printf("=== Basic Circular Buffer Tests ===\n");
    // test_create_destroy();
    // test_status_functions();
    // printf("=== Circular Buffer Put/Get Tests ===\n");
    // test_put_get_basic();
    // test_circular_behavior();
    // test_null_pointers();
    printf("=== Peek and Reset Tests ===\n");
    test_peek();
    test_reset();
    test_different_data_types();
    printf("All tests passed!\n");
    return 0;
}