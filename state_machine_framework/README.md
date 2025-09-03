# State Machine Framework for Embedded Systems

**Project 4  of 30** - *30 Days of C Programming for Embedded Systems*

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

This project implements a generic, table-driven finite state machine (FSM) framework in C, specifically designed for embedded systems. The framework provides a reusable foundation for implementing state-based control systems with deterministic behavior and debugging capabilities.

## Motivation

State machines are fundamental components in embedded systems. State machines are crucial for:

- **Real-time control systems** (traffic lights, industrial controllers)
- **Device state management** (power states, operational modes)
- **User interface logic** (menu systems, button handling)

This project demonstrates embedded software engineering concepts including system design, control logic implementation, static memory management, and table-driven architectures.

## Key Features

### Generic State Machine Framework
- **Static memory allocation** 
- **Table-driven design** 
- **Type-safe state and event definitions** 
- **Comprehensive error handling** 
- **Optional logging system** 

### Traffic Light Controller Example
- **European-style traffic light** implementation (RED → RED+YELLOW → GREEN → YELLOW)
- **Interactive control** - Manual event triggering and automatic simulation
- **Emergency and reset handling** - Safety-critical functionality
- **Real-time status monitoring** - State transitions with timing
- **Statistics tracking** - Transition counts and invalid event monitoring

## Architecture

### Core Components

```c
typedef struct {
    char id[SM_MAX_ID_LENGTH];
    bool initialized;
    sm_state_t current_state;
    sm_state_t initial_state;
    
    const sm_state_tab_t *state_table;
    uint8_t num_states;
    
    const sm_transition_tab_t *transition_table;
    uint8_t num_transitions;
    
    bool logging_enabled;
    uint32_t transition_count;
    uint32_t invalid_event_count;
} state_machine_t;
```

### State Definition Structure
```c
typedef struct {
    sm_state_t state;
    sm_state_fn_t on_entry;    // Called when entering state
    sm_state_fn_t on_exit;     // Called when leaving state
    const char *name;          // Human-readable name for debugging
} sm_state_tab_t;
```

### Transition Table Structure
```c
typedef struct {
    sm_state_t from_state;     // Source state
    sm_event_t event;          // Triggering event
    sm_state_t to_state;       // Destination state
    sm_action_fn_t action;     // Optional transition action
} sm_transition_tab_t;
```

## API Reference

### Core Functions

#### `sm_init()`
Initializes a state machine with configuration tables.
```c
sm_result_t sm_init(state_machine_t *sm, 
                    const char *id, 
                    sm_state_t initial_state,
                    const sm_state_tab_t *state_table, 
                    uint8_t num_states,
                    const sm_transition_tab_t *transition_table, 
                    uint8_t num_transitions);
```

#### `sm_process_event()`
Processes an event and executes state transition if valid.
```c
sm_result_t sm_process_event(state_machine_t *sm, sm_event_t event);
```

#### `sm_reset()`
Resets state machine to initial state.
```c
sm_result_t sm_reset(state_machine_t *sm);
```

### Utility Functions

#### `sm_is_in_state()`
Checks if state machine is in a specific state.
```c
bool sm_is_in_state(const state_machine_t *sm, sm_state_t state);
```

#### `sm_get_current_state()`
Retrieves current state safely.
```c
sm_result_t sm_get_current_state(const state_machine_t *sm, sm_state_t *current_state);
```

### Debugging Functions

#### `sm_set_logging()`
Enables or disables transition logging.
```c
void sm_set_logging(state_machine_t *sm, bool enable);
```

#### `sm_print_status()`
Displays comprehensive state machine status.
```c
void sm_print_status(const state_machine_t *sm);
```

#### `sm_get_stats()`
Retrieves transition statistics.
```c
sm_result_t sm_get_stats(const state_machine_t *sm, 
                         uint32_t *total_transitions, 
                         uint32_t *invalid_events);
```

## Project Structure

```
state_machine_framework/
├── include/
│   └── state_machine.h        # Framework header with API definitions
├── src/
│   └── state_machine.c        # Core framework implementation
├── examples/
│   └── traffic_light.c        # Traffic light controller example
├── tests/
│   └── (future test files)
├── Makefile                   # Build system
└── README.md                  # This documentation
```

## Building and Running

### Prerequisites
- GCC compiler
- Make utility

### Quick Start

```bash
# Clone the repository
git clone [this_repos's_url]
cd state_machine_framework

# Build and run the traffic light example
make run
```

### Build Options

```bash
# Standard build
make

# Clean build artifacts
make clean

# Display help
make help
```

## Usage Example

### Defining States
```c
typedef enum {
    STATE_IDLE = 0,
    STATE_PROCESSING,
    STATE_COMPLETE
} my_states_t;

const sm_state_tab_t my_state_table[] = {
    {STATE_IDLE,       idle_entry,       idle_exit,       "IDLE"},
    {STATE_PROCESSING, processing_entry, processing_exit, "PROCESSING"},
    {STATE_COMPLETE,   complete_entry,   NULL,            "COMPLETE"}
};
```

### Defining Transitions
```c
typedef enum {
    EVENT_START = 0,
    EVENT_FINISH,
    EVENT_RESET
} my_events_t;

const sm_transition_tab_t my_transitions[] = {
    {STATE_IDLE,       EVENT_START,  STATE_PROCESSING, start_action},
    {STATE_PROCESSING, EVENT_FINISH, STATE_COMPLETE,   finish_action},
    {STATE_COMPLETE,   EVENT_RESET,  STATE_IDLE,       reset_action}
};
```

### Using the State Machine
```c
state_machine_t my_sm;

// Initialize
sm_result_t result = sm_init(&my_sm, "MyController", STATE_IDLE,
                             my_state_table, 3,
                             my_transitions, 3);

// Process events
sm_process_event(&my_sm, EVENT_START);
sm_process_event(&my_sm, EVENT_FINISH);
```

## Traffic Light Example

### Interactive Commands
- **SPACE/ENTER**: Timer expired (normal traffic flow)
- **e**: Emergency stop (immediate RED)
- **r**: Reset system
- **s**: Show detailed status
- **l**: Toggle logging
- **a**: Automatic 30-second simulation
- **h**: Help menu
- **q**: Quit

### State Flow
```
RED (3s) → RED+YELLOW (1s) → GREEN (4s) → YELLOW (2s) → RED
```

### Emergency Handling
From any state except RED, emergency stop immediately transitions to RED state with appropriate logging and actions.

## Technical Challenges and Solutions

### Challenge 1: Static vs Dynamic Memory Management
**Problem**: Embedded systems require predictable memory usage, but initial design used dynamic allocation.

**Solution**: Redesigned to use static allocation where the caller provides the state machine structure, and all tables are defined at compile-time. This eliminates malloc/free calls and provides deterministic memory usage.

### Challenge 2: Function Pointer Declaration Order
**Problem**: Circular dependency between struct definitions and function pointer typedefs.

**Solution**: Used forward declarations and careful ordering of type definitions. Function pointer types are declared before structs that use them, with forward declaration of the main struct type.

### Challenge 3: String Handling in Embedded Context
**Problem**: Safe string handling without dynamic allocation for state names and logging.

**Solution**: Used fixed-size character arrays with proper bounds checking and null termination. Implemented safe string copying with strncpy() and manual null termination.

### Challenge 5: Error Handling Strategy
**Problem**: Balancing comprehensive error checking with performance requirements.

**Solution**: Designed hierarchical error codes that provide specific failure information while maintaining fast execution paths for normal operation. All functions return error codes with consistent patterns.

## Learning Outcomes

This project reinforced several embedded software engineering concepts:

### System Design
- State machine patterns and their applications
- Table-driven architecture benefits
- Static vs dynamic memory trade-offs
- API design for embedded systems

### C Programming Skills
- Function pointers and callbacks
- Struct organization and forward declarations
- String handling without dynamic allocation
- Comprehensive error handling patterns

### Development Practices
- Makefile creation for complex projects
- Code organization and modular design
- Documentation and API specification

## Future Enhancements

- Hierarchical state machines support
- Additional example implementations
- 
## 🤝 Contributing

While this is a learning project, feedback and suggestions are welcome! Please feel free to:
- Report bugs or issues
- Suggest improvements

## 📞 Contact

**Author**: [Melek GUEDIDI]  
**Email**: [melek.guedidi@insat.ucar.tn]  
**LinkedIn**: [https://www.linkedin.com/in/melek-guedidi/]  

---

**Part of the 30-Day Embedded C Challenge** | **Project 4/30** | **Next**: CRC Calculation Library

*Building embedded systems expertise one project at a time* 🚀
