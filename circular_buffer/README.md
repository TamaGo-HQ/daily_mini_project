# 🔧 Circular Buffer Library for Embedded Systems

**Project 3 of 30** - *30 Days of C Programming for Embedded Systems*

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A high-performance, thread-safe circular buffer (ring buffer) implementation in C that provides FIFO data storage with fixed memory allocation. This project demonstrates C programming concepts including memory management, thread synchronization, and cross-platform compatibility.

## Key Features

- **Thread-Safe**: Mutex-based synchronization for concurrent access
- **Cross-Platform**: Works on Windows and Unix-like systems
- **Generic**: Supports any data type through configurable element size
- **Memory Efficient**: Fixed allocation with no dynamic resizing
- **Comprehensive Error Handling**: Detailed return codes for all operations
- **Production Ready**: Robust validation and edge case handling

## API Reference

### Core Functions

#### Buffer Management
```c
circular_buffer_t* cb_create(size_t capacity, size_t element_size);
void cb_destroy(circular_buffer_t* cb);
```

#### Data Operations
```c
cb_result_t cb_put(circular_buffer_t* cb, const void* data);    // Add element
cb_result_t cb_get(circular_buffer_t* cb, void* data);         // Remove element  
cb_result_t cb_peek(circular_buffer_t* cb, void* data);        // Read without removing
```

#### Status Queries
```c
bool cb_is_empty(const circular_buffer_t* cb);
bool cb_is_full(const circular_buffer_t* cb);
size_t cb_count(const circular_buffer_t* cb);
size_t cb_capacity(const circular_buffer_t* cb);
size_t cb_free_space(const circular_buffer_t* cb);
```

#### Utilities
```c
void cb_reset(circular_buffer_t* cb);                          // Clear buffer
void cb_reset_secure(circular_buffer_t* cb);                  // Clear + zero memory
```

### Return Codes
```c
typedef enum {
    CB_SUCCESS = 0,
    CB_ERROR_NULL_POINTER,
    CB_ERROR_FULL,
    CB_ERROR_EMPTY,
    CB_ERROR_INVALID_SIZE,
    CB_ERROR_MEMORY_ALLOCATION,
    CB_ERROR_MUTEX_INIT,
    CB_ERROR_MUTEX_LOCK
} cb_result_t;
```

## Quick Start

### Prerequisites
- GCC or compatible C compiler
- Make utility
- pthread library (Unix/Linux)
- Windows SDK (Windows)

### Building the Project
```bash
# Clone the repository
git clone <this_repos's_url>
cd circular-buffer

# Build all targets
make all

# Run tests
make test

# Memory leak checking (requires valgrind)
make memcheck

# Clean build artifacts
make clean
```

### Basic Usage Example
```c
#include "circular_buffer.h"

int main() {
    // Create buffer for 10 integers
    circular_buffer_t *buffer = cb_create(10, sizeof(int));
    
    // Add data
    int data = 42;
    if (cb_put(buffer, &data) == CB_SUCCESS) {
        printf("Data added successfully\n");
    }
    
    // Read data
    int result;
    if (cb_get(buffer, &result) == CB_SUCCESS) {
        printf("Retrieved: %d\n", result);
    }
    
    // Cleanup
    cb_destroy(buffer);
    return 0;
}
```

## Testing

The project includes comprehensive thread-safety tests:

### Test Coverage
- **Basic functionality**: FIFO behavior, boundary conditions
- **Thread safety**: Concurrent producers and consumers
- **Data integrity**: No corruption under concurrent access
- **Error handling**: All error conditions properly tested

### Running Tests
```bash
# Standard test run
make test

# Memory leak detection
make memcheck

```

## 🔧 Project Structure

```
circular-buffer/
├── include/
│   └── circular_buffer.h      # Header with API declarations
├── src/
│   └── circular_buffer.c      # Implementation
├── tests/
│   └── test_threadsafe.c      # Comprehensive test suite
├── Makefile                   # Build configuration
└── README.md                  # This documentation
```

## Design Decisions

### Thread Safety Strategy
- **Mutex per buffer**: Each buffer has its own mutex
- **Cross-platform abstraction**: Windows Critical Sections vs. pthread mutexes

### Memory Management
- **Fixed allocation**: No dynamic resizing to avoid fragmentation
- **Generic element handling**: Supports any data type via `void*` and element size
- **Secure cleanup**: Optional memory zeroing for sensitive data

### Error Handling
- **Comprehensive return codes**: Every operation returns detailed status

## Implementation Challenges

### 1. Cross-Platform Compatibility
**Challenge**: Different threading primitives on Windows vs. Unix systems.

**Solution**:
- Conditional compilation using preprocessor directives
- Abstracted mutex types and operations

### 3. Generic Data Type Support
**Challenge**: Supporting arbitrary data types in C 

**Solution**:
- Used `void*` pointers with configurable element sizes
- `memcpy` for type-agnostic data copying

### 4. Memory Management Edge Cases
**Challenge**: Preventing memory leaks and handling allocation failures gracefully.

**Solution**:
- Implemented cleanup in `cb_destroy()`
- Added allocation failure handling in `cb_create()`
- Provided secure memory clearing option

### 5. Producer-Consumer Synchronization
**Challenge**: Efficient coordination between multiple producers and consumers without busy-waiting.

**Solution**:
- Proper error codes (`CB_ERROR_FULL`, `CB_ERROR_EMPTY`) for flow control
- Thread-safe status queries

## Future Enhancements

- **Batch operations**: Multi-element put/get for improved performance
- **Lock-free implementation**: Using atomic operations for higher performance
- **Statistics collection**: Performance metrics and usage patterns

## Learning Outcomes

- Thread synchronization and concurrent programming
- Cross-platform development practices
- Memory management
- Data structure implementation and optimization
- Comprehensive testing strategies

## 🤝 Contributing

While this is a learning project, feedback and suggestions are welcome! Please feel free to:
- Report bugs or issues
- Suggest improvements

## 📞 Contact

**Author**: [Melek GUEDIDI]  
**Email**: [melek.guedidi@insat.ucar.tn]  
**LinkedIn**: [https://www.linkedin.com/in/melek-guedidi/]  

---

**Part of the 30-Day Embedded C Challenge** | **Project 3/30** | **Next**: State Machine FrameWork
*Building embedded systems expertise one project at a time* 🚀
