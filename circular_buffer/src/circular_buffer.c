#include "circular_buffer.h"
#include <stdlib.h>  // for malloc, free
#include <string.h>  // for memcpy, memset

// core functions
circular_buffer_t* cb_create(size_t capacity, size_t element_size){
    if(capacity == 0 || element_size ==0){
        return NULL;
    }

    // allocate memory for the struct
    circular_buffer_t *cb = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
    if(!cb) return NULL;

    //allocate memeory for the data buffer
    cb->buffer = malloc(capacity * element_size);
    if (!cb->buffer){
        free(cb);
        return NULL;
    }

    // ititialize fields;
    cb->capacity = capacity;
    cb->element_size = element_size;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;

    return cb;
}

void cb_destroy(circular_buffer_t* cb){
    if (cb){
        free(cb->buffer);
        free(cb);
    }  
}

//status query functions
bool cb_is_empty(const circular_buffer_t* cb){
    if(cb == NULL || cb->count == 0){
        return true;
    }
    return false;
}

bool cb_is_full(const circular_buffer_t* cb){
    if(cb==NULL) return false;
    if(cb->count == cb->capacity){
        return true;
    }
    return false;
}

size_t cb_count(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    return cb->count;
}

size_t cb_capacity(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    return cb->capacity;
}

size_t cb_free_space(const circular_buffer_t* cb){
    if(cb == NULL) return 0; //how do i signal this?
    return cb->capacity - cb->count;
}

cb_result_t cb_put(circular_buffer_t* cb, const void* data){//why are we passing *data instead of just data, we're n ot changing it anyway
    //validate inputs
    if(cb == NULL) return CB_ERROR_NULL_POINTER;
    if(data == NULL) return CB_ERROR_NULL_POINTER;

    //check if buffer is full
    if(cb_is_full(cb)) return CB_ERROR_FULL;
    
    //calculate destination @ in buffer
    void *dest = cb->buffer + (cb->head * cb->element_size);

    //copy data to buffer
    memcpy(dest, data, cb->element_size); 

    cb->head = (cb->head + 1) % cb->capacity;
    cb->count++;

    return CB_SUCCESS;
}

cb_result_t cb_get(circular_buffer_t* cb, void *data){
    // Validate inputs
    if (cb == NULL) return CB_ERROR_NULL_POINTER;
    if (data == NULL) return CB_ERROR_NULL_POINTER;
    
    // Check if buffer is empty
    if (cb_is_empty(cb)) return CB_ERROR_EMPTY;

    // Calculate source @ in buffer
    void *src = cb->buffer + (cb->tail * cb->element_size);
    
    // Copy data from buffer
    memcpy(data, src, cb->element_size);
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count--;

    return CB_SUCCESS;
}

cb_result_t cb_peek(circular_buffer_t* cb, void* data){
    // Validate inputs
    if (cb == NULL) return CB_ERROR_NULL_POINTER;
    if (data == NULL) return CB_ERROR_NULL_POINTER;
    
    // Check if buffer is empty
    if (cb_is_empty(cb)) return CB_ERROR_EMPTY;

    // Calculate source @ in buffer
    void *src = cb->buffer + (cb->tail * cb->element_size);
    
    // Copy data from buffer
    memcpy(data, src, cb->element_size);

    return CB_SUCCESS;
}

// Utility function
void cb_reset(circular_buffer_t* cb) {
    if (cb == NULL) return;
    
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    // We don't need to clear the actual buffer data
    // since count=0 means it's logically empty
}

// Alternative: If we want to actually clear the buffer data
void cb_reset_secure(circular_buffer_t* cb) {
    if (cb == NULL) return;
    
    // Clear the buffer memory (useful for sensitive data)
    memset(cb->buffer, 0, cb->capacity * cb->element_size);
    
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}
