#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include<stdint.h>

typedef enum{
    SM_SUCCESS = 0,
    SM_ERROR_NULL_POINTER,
    SM_ERROR_INVALID_STATE,
    SM_ERROR_INVALID_EVENT,
} sm_result_t;

typedef uint8_t sm_state_t;
typedef uint8_t sm_event_t;

typedef struct{
    sm_state_t from_state;
    sm_state_t to_state;
    sm_event_t event;
    sm_action_fn_t action;
} sm_transition_tab_t;

typedef struct{
    sm_state_t state;
    sm_state_fn_t on_entry;
    sm_state_fn_t on_exit;
    const char *name;
} sm_state_tab_t;

typedef struct{
    char* id;
    bool initialized;
    sm_state_t current_state;
    sm_state_t initial_state;

    const sm_state_tab_t *state_table;
    uint8_t num_states;

    const sm_transition_tab_t *transition_table;
    uint8_t num_transitions;

    bool logging_enabled;
}state_machine_t;

typedef void (*sm_action_fn_t)(state_machine_t* sm, sm_state_t from, sm_state_t to, sm_event_t event);
typedef void (*sm_state_fn_t)(state_machine_t *sm, sm_state_t state);

// Core state machine api

sm_result_t sm_init(state_machine_t *sm, const char *id, sm_state_t initial_state, const sm_state_tab_t *state_table, uint8_t num_states, const sm_transition_tab_t);
sm_result_t sm_reset(state_machine_t *sm);
sm_result_t sm_process_event(state_machine_t *sm, sm_event_t event);

//status queries

sm_result_t sm_get_current_state(state_machine_t *sm, sm_state_t *current_state);
bool sm_is_in_state(const state_machine_t *sm, sm_state_t state);
const char *sm_get_state_name(const state_machine_t *sm, sm_state_t state);

//logging and debugging
const char *sm_get_state_name(const state_machine_t *sm, sm_state_t state);
void sm_set_logging(state_machine_t *sm, bool enable);
void sm_print_status(const state_machine_t *sm);

#endif