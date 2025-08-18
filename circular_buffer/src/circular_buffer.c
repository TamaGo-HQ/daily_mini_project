#include "circular_buffer.h"
#include <stdlib.h>  // for malloc, free
#include <string.h>  // for memcpy, memset

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

