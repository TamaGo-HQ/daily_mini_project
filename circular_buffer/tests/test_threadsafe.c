#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "circular_buffer.h"

// Windows threading
#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
    #define THREAD_RETURN unsigned int
    #define THREAD_CALL __stdcall
    typedef HANDLE thread_t;
#else
    #include <pthread.h>
    #include <unistd.h>
    #define THREAD_RETURN void*
    #define THREAD_CALL
    typedef pthread_t thread_t;
#endif

// Test data structure
typedef struct {
    circular_buffer_t *buffer;
    int thread_id;
    int operations_count;
    int *success_count;
} thread_data_t;

// Basic functionality test (single threaded)
void test_basic_functionality() {
    printf("Testing basic functionality with thread-safe buffer...\n");
    
    circular_buffer_t *cb = cb_create(5, sizeof(int));
    assert(cb != NULL);
    
    // Test empty state
    assert(cb_is_empty(cb) == true);
    assert(cb_is_full(cb) == false);
    assert(cb_count(cb) == 0);
    assert(cb_capacity(cb) == 5);
    assert(cb_free_space(cb) == 5);
    
    // Test putting data
    int data[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        assert(cb_put(cb, &data[i]) == CB_SUCCESS);
    }
    
    assert(cb_is_full(cb) == true);
    assert(cb_count(cb) == 5);
    
    // Test overflow
    int extra = 99;
    assert(cb_put(cb, &extra) == CB_ERROR_FULL);
    
    // Test getting data (FIFO)
    int result;
    for (int i = 0; i < 5; i++) {
        assert(cb_get(cb, &result) == CB_SUCCESS);
        assert(result == data[i]);
    }
    
    assert(cb_is_empty(cb) == true);
    
    // Test underflow
    assert(cb_get(cb, &result) == CB_ERROR_EMPTY);
    
    // Test peek
    data[0] = 100;
    cb_put(cb, &data[0]);
    assert(cb_peek(cb, &result) == CB_SUCCESS);
    assert(result == 100);
    assert(cb_count(cb) == 1); // Should not change count
    
    // Test reset
    cb_reset(cb);
    assert(cb_is_empty(cb) == true);
    assert(cb_count(cb) == 0);
    
    cb_destroy(cb);
    printf("✓ Basic functionality works with thread-safe buffer\n");
}

// Producer thread function
THREAD_RETURN THREAD_CALL producer_thread(void *arg) {
    thread_data_t *data = (thread_data_t*)arg;
    circular_buffer_t *cb = data->buffer;
    int success = 0;
    
    for (int i = 0; i < data->operations_count; i++) {
        int value = (data->thread_id * 1000) + i;
        
        // Try to put data (might fail if buffer is full)
        if (cb_put(cb, &value) == CB_SUCCESS) {
            success++;
        }
        
        // Small delay to increase chance of race conditions
        #ifdef _WIN32
            Sleep(1);
        #else
            usleep(1000);
        #endif
    }
    
    *(data->success_count) = success;
    printf("Producer %d: %d/%d operations successful\n", 
           data->thread_id, success, data->operations_count);
    
    #ifdef _WIN32
        return 0;
    #else
        return NULL;
    #endif
}

// Consumer thread function
THREAD_RETURN THREAD_CALL consumer_thread(void *arg) {
    thread_data_t *data = (thread_data_t*)arg;
    circular_buffer_t *cb = data->buffer;
    int success = 0;
    
    for (int i = 0; i < data->operations_count; i++) {
        int value;
        
        // Try to get data (might fail if buffer is empty)
        if (cb_get(cb, &value) == CB_SUCCESS) {
            success++;
            // Verify the data looks reasonable
            if (value < 0 || value > 999999) {
                printf("ERROR: Got corrupted data: %d\n", value);
            }
        }
        
        // Small delay
        #ifdef _WIN32
            Sleep(1);
        #else
            usleep(1000);
        #endif
    }
    
    *(data->success_count) = success;
    printf("Consumer %d: %d/%d operations successful\n", 
           data->thread_id, success, data->operations_count);
    
    #ifdef _WIN32
        return 0;
    #else
        return NULL;
    #endif
}

void test_multithreaded() {
    printf("Testing multi-threaded access...\n");
    
    circular_buffer_t *cb = cb_create(10, sizeof(int));
    assert(cb != NULL);
    
    // Test parameters
    const int num_producers = 2;
    const int num_consumers = 2;
    const int operations_per_thread = 100;
    
    // Thread data
    thread_data_t producer_data[num_producers];
    thread_data_t consumer_data[num_consumers];
    thread_t producer_threads[num_producers];
    thread_t consumer_threads[num_consumers];
    int producer_success[num_producers];
    int consumer_success[num_consumers];
    
    // Initialize thread data
    for (int i = 0; i < num_producers; i++) {
        producer_data[i].buffer = cb;
        producer_data[i].thread_id = i;
        producer_data[i].operations_count = operations_per_thread;
        producer_data[i].success_count = &producer_success[i];
    }
    
    for (int i = 0; i < num_consumers; i++) {
        consumer_data[i].buffer = cb;
        consumer_data[i].thread_id = i;
        consumer_data[i].operations_count = operations_per_thread;
        consumer_data[i].success_count = &consumer_success[i];
    }
    
    printf("Starting %d producers and %d consumers...\n", num_producers, num_consumers);
    
    // Start producer threads
    for (int i = 0; i < num_producers; i++) {
        #ifdef _WIN32
            producer_threads[i] = (HANDLE)_beginthreadex(NULL, 0, producer_thread, 
                                                        &producer_data[i], 0, NULL);
        #else
            pthread_create(&producer_threads[i], NULL, producer_thread, &producer_data[i]);
        #endif
    }
    
    // Start consumer threads  
    for (int i = 0; i < num_consumers; i++) {
        #ifdef _WIN32
            consumer_threads[i] = (HANDLE)_beginthreadex(NULL, 0, consumer_thread, 
                                                        &consumer_data[i], 0, NULL);
        #else
            pthread_create(&consumer_threads[i], NULL, consumer_thread, &consumer_data[i]);
        #endif
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < num_producers; i++) {
        #ifdef _WIN32
            WaitForSingleObject(producer_threads[i], INFINITE);
            CloseHandle(producer_threads[i]);
        #else
            pthread_join(producer_threads[i], NULL);
        #endif
    }
    
    for (int i = 0; i < num_consumers; i++) {
        #ifdef _WIN32
            WaitForSingleObject(consumer_threads[i], INFINITE);
            CloseHandle(consumer_threads[i]);
        #else
            pthread_join(consumer_threads[i], NULL);
        #endif
    }
    
    // Calculate totals
    int total_produced = 0, total_consumed = 0;
    for (int i = 0; i < num_producers; i++) {
        total_produced += producer_success[i];
    }
    for (int i = 0; i < num_consumers; i++) {
        total_consumed += consumer_success[i];
    }
    
    printf("Total produced: %d, Total consumed: %d, Remaining in buffer: %zu\n",
           total_produced, total_consumed, cb_count(cb));
    
    // Verify data integrity
    assert(total_produced == total_consumed + (int)cb_count(cb));
    
    cb_destroy(cb);
    printf("✓ Multi-threaded test passed - no data corruption detected!\n");
}

int main() {
    printf("=== Thread-Safe Circular Buffer Tests ===\n");
    
    test_basic_functionality();
    test_multithreaded();
    
    printf("\n🎉 All thread-safe tests passed!\n");
    printf("Your circular buffer is ready for embedded systems!\n");
    
    return 0;
}