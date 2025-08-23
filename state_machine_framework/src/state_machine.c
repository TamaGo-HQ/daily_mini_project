#include "state_machine.h"

//helper
static bool is_valid_state(state_machine_t* sm, sm_state_t state){
    if (!sm || !sm->state_table) return false;
    for (uint8_t i = 0; i < sm->num_states; i++){
        if(sm->state_table[i].state == state)
            return true;
    }
        return false;
}
static const sm_state_tab_t* find_state_def(const state_machine_t* sm, sm_state_t state){
    if(!sm || !sm->state_table) return NULL;
    for (uint8_t i = 0; i < sm->num_states; i++){
        if(sm->state_table[i].state == state){
            return &sm->state_table[i];
        }
    }
    return NULL;
}
static const sm_transition_tab_t *find_transition_def(const state_machine_t *sm,sm_state_t state, sm_event_t event){
    //look for where the state corresponds to current state
    //then check if event corresponds
    if(!sm || !sm->transition_table) return NULL; 
    for (uint8_t i = 0; i < sm->num_transitions; i++){
        if((sm->transition_table[i].from_state == state) && (sm->transition_table[i].event == event)){
            return &sm->transition_table[i];
        }
    }
    return NULL;
}

//core
sm_result_t sm_init(state_machine_t *sm, const char *id, sm_state_t initial_state, const sm_state_tab_t *state_table, uint8_t num_states, const sm_transition_tab_t * transition_table, uint8_t num_transitions){
    if(sm == NULL){
        return SM_ERROR_NULL_POINTER;
    }

    if(id == NULL || state_table == NULL || transition_table == NULL){
        return SM_ERROR_NULL_POINTER;
    }

    if(num_states == 0 || num_states > SM_MAX_STATES){
        return SM_ERROR_INVALID_STATE;
    }

    if(num_transitions >SM_MAX_TRANSITIONS){
        return SM_ERROR_TABLE_FULL;
    }

    memset(sm, 0, sizeof(state_machine_t));

    strncpy(sm->id, id, SM_MAX_ID_LENGTH - 1);
    sm->id[SM_MAX_ID_LENGTH - 1] = '\0';

    sm->initial_state = initial_state;
    sm->current_state = initial_state;
    sm->state_table = state_table;
    sm->num_states = num_states;
    sm->transition_table = transition_table;
    sm->num_transitions = num_transitions;

    sm->logging_enabled = false;
    sm->transition_count = 0;
    sm->invalid_event_count = 0;

    if (!is_valid_state(sm, initial_state)){
        return SM_ERROR_INVALID_STATE;
    }

    sm->initialized = true;

    const sm_state_tab_t *state_def = find_state_def(sm, initial_state);
    if (state_def && state_def->on_entry) {
        state_def->on_entry(sm, initial_state);
    } 

    if (sm->logging_enabled) {
        printf("[SM:%s] Initialized - Initial state : %s\n",sm->id, sm_get_state_name(sm, initial_state));

    }

    return SM_SUCCESS;
}
sm_result_t sm_reset(state_machine_t *sm){
    if(!sm) return SM_ERROR_NULL_POINTER; 
    if(!sm->initialized) return SM_ERROR_NOT_INITIALIZED;

    sm_state_t old_state = sm->current_state;

    if(old_state != sm->initial_state){
        const sm_state_tab_t *current_state_def = find_state_def(sm, old_state);
        if(current_state_def && current_state_def->on_exit){
            current_state_def->on_exit(sm, old_state);
        }
    }

    sm->current_state = sm->initial_state;
    sm->transition_count = 0;
    sm->invalid_event_count = 0;

    if(old_state != sm->initial_state){
        const sm_state_tab_t *initial_state_def = find_state_def(sm, sm->initial_state);
        if(initial_state_def && initial_state_def->on_entry){
            initial_state_def->on_entry(sm, sm->initial_state);
        }
    }

    if(sm->logging_enabled){
        printf("[SM:%s] Reset from %s to %s\n", sm->id, sm_get_state_name(sm, old_state),
               sm_get_state_name(sm, sm->initial_state));
    }

    return SM_SUCCESS;
}
sm_result_t sm_process_event(state_machine_t* sm, sm_event_t event){
    //check sm is not null
    if(!sm) return SM_ERROR_NULL_POINTER;
    const sm_transition_tab_t *transition = find_transition_def(sm, sm->current_state, event);
    if(transition == NULL){
        sm->invalid_event_count++;
        return SM_ERROR_INVALID_EVENT;  
    }  
    const sm_state_tab_t *old_state_def = find_state_def(sm, sm->current_state);
    if(old_state_def && old_state_def->on_exit){
        old_state_def->on_exit(sm, sm->current_state);
    }
    //do transition action
    if(transition->action){
        transition->action(sm, transition->from_state, transition->to_state, transition->event);
    }

    sm->current_state = transition->to_state;
    sm->transition_count++;

    //do on entry to new state action 
    const sm_state_tab_t *new_state_def = find_state_def(sm, sm->current_state);
    if(new_state_def && new_state_def->on_entry){
        new_state_def->on_entry(sm, sm->current_state);
    }

    // Log transition if logging is enabled
    if (sm->logging_enabled) {
        printf("[SM:%s] Transition: %s -> %s (event: %d)\n", 
               sm->id, 
               old_state_def->name, 
               sm_get_state_name(sm, sm->current_state),
               event);
    }

    return SM_SUCCESS;
    //return error
}

// utility
bool sm_is_in_state(const state_machine_t *sm, sm_state_t state) {
    if (!sm) return false;
    return (sm->current_state == state);  // ✅ Simplified
}



//logging and debugging
const char *sm_get_state_name(const state_machine_t *sm, sm_state_t state) {
    if (!sm) return "NULL_SM";
    
    const sm_state_tab_t* state_def = find_state_def(sm, state);  
    if (!state_def || !state_def->name) return "UNKNOWN";
    
    return state_def->name;
}
void sm_set_logging(state_machine_t *sm, bool enable){
    if(!sm) return;
    sm->logging_enabled = enable;
}
void sm_print_status(const state_machine_t *sm) {
    if (!sm) {
        printf("State Machine: NULL\n");
        return;
    }
    
    printf("=== State Machine Status ===\n");
    printf("ID: %s\n", sm->id);
    printf("Initialized: %s\n", sm->initialized ? "Yes" : "No");
    printf("Current State: %s\n", sm_get_state_name(sm, sm->current_state));
    printf("Initial State: %s\n", sm_get_state_name(sm, sm->initial_state));
    printf("Number of States: %d\n", sm->num_states);
    printf("Number of Transitions: %d\n", sm->num_transitions);
    printf("Logging Enabled: %s\n", sm->logging_enabled ? "Yes" : "No");
    printf("Transition Count: %lu\n", (unsigned long)sm->transition_count);
    printf("Invalid Event Count: %lu\n", (unsigned long)sm->invalid_event_count);
    printf("============================\n");
}
sm_result_t sm_get_current_state(const state_machine_t *sm, sm_state_t *current_state) {
    if (!sm || !current_state) {
        return SM_ERROR_NULL_POINTER;
    }
    
    if (!sm->initialized) {
        return SM_ERROR_NOT_INITIALIZED;
    }
    
    *current_state = sm->current_state;
    return SM_SUCCESS;
}

sm_result_t sm_get_stats(const state_machine_t *sm, uint32_t *total_transitions, uint32_t *invalid_events) {
    if (!sm || !total_transitions || !invalid_events) {
        return SM_ERROR_NULL_POINTER;
    }
    
    if (!sm->initialized) {
        return SM_ERROR_NOT_INITIALIZED;
    }
    
    *total_transitions = sm->transition_count;
    *invalid_events = sm->invalid_event_count;
    return SM_SUCCESS;
}