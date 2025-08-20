#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h> //for size_t
#include <stdbool.h> //for bool

// Windows mutex
#ifdef _WIN32
    #include <windows.h>
    typedef CRITICAL_SECTION cb_mutex_t;
#else
    #include <pthread.h>
    typedef pthread_mutex_t cb_mutex_t;
#endif

// Return codes for operations
typedef enum{
    CB_SUCCESS = 0,
    CB_ERROR_NULL_POINTER,
    CB_ERROR_FULL,
    CB_ERROR_EMPTY,
    CB_ERROR_INVALID_SIZE,
    CB_ERROR_MEMORY_ALLOCATION,
    CB_ERROR_MUTEX_INIT,
    CB_ERROR_MUTEX_LOCK
} cb_result_t;

// Circular buffer structure
typedef struct{
    uint8_t *buffer;     // Pointer to data structure
    size_t capacity;     // Maximum number of elements
    size_t element_size; // Size of each element
    size_t head;         // Write position (producer index)
    size_t tail;         // Read poistion (consumer index)
    size_t count;        // Current number of elements
    cb_mutex_t lock;        // Mutex for thread safety
}circular_buffer_t;

// Core API functions
circular_buffer_t* cb_create(size_t capacity, size_t element_size);
void cb_destroy(circular_buffer_t* cb);

// Data operations
cb_result_t cb_put(circular_buffer_t* cb, const void* data);
cb_result_t cb_get(circular_buffer_t* cb, void* data);
cb_result_t cb_peek(circular_buffer_t* cb, void* data);

// Status queries
bool cb_is_empty (const circular_buffer_t* cb);
bool cb_is_full(const circular_buffer_t* cb);
size_t cb_count(const circular_buffer_t* cb);
size_t cb_capacity(const circular_buffer_t* cb);
size_t cb_free_space(const circular_buffer_t* cb);

// Utility functions
void cb_reset(circular_buffer_t* cb);

#endif //CIRCULAR_BUFFER_H