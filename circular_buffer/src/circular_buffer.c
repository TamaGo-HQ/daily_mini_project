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

    #ifdef _WIN32
        InitializeCriticalSection(&cb->lock);
    #else
        pthread_mutex_init(&cb->lock, NULL);
    #endif

    return cb;
}

void cb_destroy(circular_buffer_t* cb){
    if (cb){
        #ifdef _WIN32
            DeleteCriticalSection(&cb->lock);
        #else
            pthread_mutex_destroy(&cb->lock);
        #endif
        free(cb->buffer);
        free(cb);
    }  
}

//public thread-safe satus query functions
bool cb_is_empty(const circular_buffer_t* cb){
    if (cb == NULL) return true;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    bool result = (cb->count == 0);
    
    #ifdef _WIN32
        LeaveCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_unlock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    return result;
}

bool cb_is_full(const circular_buffer_t* cb){
    if (cb == NULL) return false;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    bool result = (cb->count == cb->capacity);
    
    #ifdef _WIN32
        LeaveCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_unlock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    return result;
}

size_t cb_count(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    size_t result = cb->count;

    #ifdef _WIN32
        LeaveCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_unlock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    return result;
}

size_t cb_capacity(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    size_t result = cb->capacity;

    #ifdef _WIN32
        LeaveCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_unlock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    return result;
}

size_t cb_free_space(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif
    
    size_t result = cb->capacity - cb->count;

    #ifdef _WIN32
        LeaveCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_unlock(&((circular_buffer_t*)cb)->lock);
    #endif

    return result;
}

// Internal helper functions (no mutex - for use inside critical sections)
bool cb_is_empty_nomutex(const circular_buffer_t* cb){
    if(cb == NULL || cb->count == 0){
        return true;
    }
    return false;
}

bool cb_is_full_nomutex(const circular_buffer_t* cb){
    if(cb==NULL) return false;
    if(cb->count == cb->capacity){
        return true;
    }
    return false;
}

size_t cb_count_nomutex(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    return cb->count;
}

size_t cb_capacity_nomutex(const circular_buffer_t* cb){
    if(cb == NULL) return 0;
    return cb->capacity;
}

size_t cb_free_space_nomutex(const circular_buffer_t* cb){
    if(cb == NULL) return 0; //how do i signal this?
    return cb->capacity - cb->count;
}


cb_result_t cb_put(circular_buffer_t* cb, const void* data){
    //validate inputs
    if(cb == NULL) return CB_ERROR_NULL_POINTER;
    if(data == NULL) return CB_ERROR_NULL_POINTER;

    cb_result_t result = CB_SUCCESS;

    // Lock the mutex
    #ifdef _WIN32
        EnterCriticalSection(&cb->lock);
    #else
        if (pthread_mutex_lock(&cb->lock) != 0) {
            return CB_ERROR_MUTEX_LOCK;
        }
    #endif

    //check if buffer is full
    if(cb_is_full_nomutex(cb)){
        result = CB_ERROR_FULL;
    }else{
        //calculate destination @ in buffer
        void *dest = cb->buffer + (cb->head * cb->element_size);
        //copy data to buffer
        memcpy(dest, data, cb->element_size); 
        cb->head = (cb->head + 1) % cb->capacity;
        cb->count++;
    }

    // Unlock the mutex
    #ifdef _WIN32
        LeaveCriticalSection(&cb->lock);
    #else
        pthread_mutex_unlock(&cb->lock);
    #endif

    return result;
}

cb_result_t cb_get(circular_buffer_t* cb, void *data){
    // Validate inputs
    if (cb == NULL) return CB_ERROR_NULL_POINTER;
    if (data == NULL) return CB_ERROR_NULL_POINTER;

    cb_result_t result = CB_SUCCESS;

    // Lock the mutex
    #ifdef _WIN32
        EnterCriticalSection(&cb->lock);
    #else
        if (pthread_mutex_lock(&cb->lock) != 0) {
            return CB_ERROR_MUTEX_LOCK;
        }
    #endif

    // Check if buffer is empty
    if (cb_is_empty_nomutex(cb)){
        result = CB_ERROR_EMPTY;
    }else{
        // Calculate source @ in buffer
        void *src = cb->buffer + (cb->tail * cb->element_size);
        
        // Copy data from buffer
        memcpy(data, src, cb->element_size);
        cb->tail = (cb->tail + 1) % cb->capacity;
        cb->count--;
    }

    // Unlock the mutex
    #ifdef _WIN32
        LeaveCriticalSection(&cb->lock);
    #else
        pthread_mutex_unlock(&cb->lock);
    #endif

    return result;
}

cb_result_t cb_peek(circular_buffer_t* cb, void* data){
    // Validate inputs
    if (cb == NULL) return CB_ERROR_NULL_POINTER;
    if (data == NULL) return CB_ERROR_NULL_POINTER;
    
    #ifdef _WIN32
        EnterCriticalSection(&((circular_buffer_t*)cb)->lock);
    #else
        pthread_mutex_lock(&((circular_buffer_t*)cb)->lock);
    #endif

    cb_result_t result = CB_SUCCESS;

    // Check if buffer is empty
    if (cb_is_empty_nomutex(cb)){
        result = CB_ERROR_EMPTY;
    }else{
        // Calculate source @ in buffer
        void *src = cb->buffer + (cb->tail * cb->element_size);
        // Copy data from buffer
        memcpy(data, src, cb->element_size);        
    }

    // Unlock the mutex
    #ifdef _WIN32
        LeaveCriticalSection(&cb->lock);
    #else
        pthread_mutex_unlock(&cb->lock);
    #endif
    
    return result;
}

// Utility function
void cb_reset(circular_buffer_t* cb) {
    if (cb == NULL) return;

    // Lock the mutex
    #ifdef _WIN32
        EnterCriticalSection(&cb->lock);
    #else
        if (pthread_mutex_lock(&cb->lock) != 0) {
            return CB_ERROR_MUTEX_LOCK;
        }
    #endif

    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    // We don't need to clear the actual buffer data
    // since count=0 means it's logically empty

    // Unlock the mutex
    #ifdef _WIN32
        LeaveCriticalSection(&cb->lock);
    #else
        pthread_mutex_unlock(&cb->lock);
    #endif
}

// Alternative: If we want to actually clear the buffer data
void cb_reset_secure(circular_buffer_t* cb) {
    if (cb == NULL) return;
    // Lock the mutex
    #ifdef _WIN32
        EnterCriticalSection(&cb->lock);
    #else
        if (pthread_mutex_lock(&cb->lock) != 0) {
            return CB_ERROR_MUTEX_LOCK;
        }
    #endif
    // Clear the buffer memory (useful for sensitive data)
    memset(cb->buffer, 0, cb->capacity * cb->element_size);
    
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    // Unlock the mutex
    #ifdef _WIN32
        LeaveCriticalSection(&cb->lock);
    #else
        pthread_mutex_unlock(&cb->lock);
    #endif
}
