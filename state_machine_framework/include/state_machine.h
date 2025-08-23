#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include<stdint.h>
#include <stdbool.h>
#include <string.h> //for memset
#include <stdio.h> //for printf

#define SM_MAX_STATES 16
#define SM_MAX_TRANSITIONS 32
#define SM_MAX_ID_LENGTH 32

typedef enum{
    SM_SUCCESS = 0,
    SM_ERROR_NULL_POINTER,
    SM_ERROR_INVALID_STATE,
    SM_ERROR_INVALID_EVENT,
    SM_ERROR_TABLE_FULL,
    SM_ERROR_NOT_INITIALIZED
} sm_result_t;

typedef uint8_t sm_state_t;
typedef uint8_t sm_event_t;
typedef struct state_machine state_machine_t;

typedef void (*sm_action_fn_t)(state_machine_t* sm, sm_state_t from, sm_state_t to, sm_event_t event);
typedef void (*sm_state_fn_t)(state_machine_t *sm, sm_state_t state);

typedef struct{
    sm_state_t from_state;
    sm_event_t event;
    sm_state_t to_state;
    sm_action_fn_t action;  // Can be NULL
} sm_transition_tab_t;

typedef struct{
    sm_state_t state;
    sm_state_fn_t on_entry;
    sm_state_fn_t on_exit;
    const char *name;
} sm_state_tab_t;

struct state_machine{
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
};


// Core state machine api

sm_result_t sm_init(state_machine_t *sm, const char *id, sm_state_t initial_state, const sm_state_tab_t *state_table, uint8_t num_states, const sm_transition_tab_t *transition_table, uint8_t num_transitions);
sm_result_t sm_reset(state_machine_t *sm);
sm_result_t sm_process_event(state_machine_t *sm, sm_event_t event);

// queries
bool sm_is_in_state(const state_machine_t *sm, sm_state_t state);

//logging and debugging
const char *sm_get_state_name(const state_machine_t *sm, sm_state_t state);
void sm_set_logging(state_machine_t *sm, bool enable);
void sm_print_status(const state_machine_t *sm);

sm_result_t sm_get_current_state(const state_machine_t *sm, sm_state_t *current_state);
sm_result_t sm_get_stats(const state_machine_t *sm, uint32_t *total_transitions, uint32_t *invalid_events);

#endif