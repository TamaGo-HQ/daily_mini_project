#include "../include/state_machine.h"
#include <stdio.h>
#include <unistd.h>  // For sleep()
#include <stdlib.h>
#include <time.h>

// ========================
// TRAFFIC LIGHT STATES
// ========================
typedef enum {
    TRAFFIC_RED = 0,
    TRAFFIC_RED_YELLOW,  // European style
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW
} traffic_state_t;

// ========================
// TRAFFIC LIGHT EVENTS
// ========================
typedef enum {
    EVENT_TIMER_EXPIRED = 0,
    EVENT_EMERGENCY_STOP,
    EVENT_RESET,
    EVENT_MAINTENANCE_MODE
} traffic_event_t;

// ========================
// TIMING CONFIGURATION
// ========================
#define RED_DURATION_SEC 3
#define RED_YELLOW_DURATION_SEC 1
#define GREEN_DURATION_SEC 4
#define YELLOW_DURATION_SEC 2

// ========================
// STATE ACTION FUNCTIONS
// ========================

// State entry functions
void on_red_entry(state_machine_t *sm, sm_state_t state) {
    printf("RED LIGHT ON - Stop! (Duration: %d seconds)\n", RED_DURATION_SEC);
}

void on_red_yellow_entry(state_machine_t *sm, sm_state_t state) {
    printf("RED + YELLOW - Prepare to go (Duration: %d seconds)\n", RED_YELLOW_DURATION_SEC);
}

void on_green_entry(state_machine_t *sm, sm_state_t state) {
    printf("GREEN LIGHT ON - Go! (Duration: %d seconds)\n", GREEN_DURATION_SEC);
}

void on_yellow_entry(state_machine_t *sm, sm_state_t state) {
    printf("YELLOW LIGHT ON - Caution! (Duration: %d seconds)\n", YELLOW_DURATION_SEC);
}

// State exit functions
void on_red_exit(state_machine_t *sm, sm_state_t state) {
    printf("Red light turning off...\n");
}

void on_green_exit(state_machine_t *sm, sm_state_t state) {
    printf("Green light turning off...\n");
}

void on_yellow_exit(state_machine_t *sm, sm_state_t state) {
    printf("Yellow light turning off...\n");
}

// ========================
// TRANSITION ACTION FUNCTIONS
// ========================

void emergency_action(state_machine_t *sm, sm_state_t from, sm_state_t to, sm_event_t event) {
    printf("EMERGENCY STOP ACTIVATED!\n");
}

void normal_transition_action(state_machine_t *sm, sm_state_t from, sm_state_t to, sm_event_t event) {
    printf("   --> Normal traffic flow transition\n");
}

void reset_action(state_machine_t *sm, sm_state_t from, sm_state_t to, sm_event_t event) {
    printf("Traffic light system reset\n");
}

// ========================
// TRAFFIC LIGHT CONFIGURATION TABLES
// ========================

// State definition table
const sm_state_tab_t traffic_states[] = {
    {TRAFFIC_RED,        on_red_entry,        on_red_exit,    "RED"},
    {TRAFFIC_RED_YELLOW, on_red_yellow_entry, NULL,           "RED_YELLOW"},
    {TRAFFIC_GREEN,      on_green_entry,      on_green_exit,  "GREEN"},
    {TRAFFIC_YELLOW,     on_yellow_entry,     on_yellow_exit, "YELLOW"}
};

// Transition table
const sm_transition_tab_t traffic_transitions[] = {
    // Normal traffic flow cycle
    {TRAFFIC_RED,        EVENT_TIMER_EXPIRED, TRAFFIC_RED_YELLOW, normal_transition_action},
    {TRAFFIC_RED_YELLOW, EVENT_TIMER_EXPIRED, TRAFFIC_GREEN,      normal_transition_action},
    {TRAFFIC_GREEN,      EVENT_TIMER_EXPIRED, TRAFFIC_YELLOW,     normal_transition_action},
    {TRAFFIC_YELLOW,     EVENT_TIMER_EXPIRED, TRAFFIC_RED,        normal_transition_action},
    
    // Emergency transitions from any state to RED
    {TRAFFIC_RED_YELLOW, EVENT_EMERGENCY_STOP, TRAFFIC_RED, emergency_action},
    {TRAFFIC_GREEN,      EVENT_EMERGENCY_STOP, TRAFFIC_RED, emergency_action},
    {TRAFFIC_YELLOW,     EVENT_EMERGENCY_STOP, TRAFFIC_RED, emergency_action},
    
    // Reset transitions from any state to RED
    {TRAFFIC_RED,        EVENT_RESET, TRAFFIC_RED,        reset_action},
    {TRAFFIC_RED_YELLOW, EVENT_RESET, TRAFFIC_RED,        reset_action},
    {TRAFFIC_GREEN,      EVENT_RESET, TRAFFIC_RED,        reset_action},
    {TRAFFIC_YELLOW,     EVENT_RESET, TRAFFIC_RED,        reset_action}
};

#define NUM_TRAFFIC_STATES (sizeof(traffic_states) / sizeof(traffic_states[0]))
#define NUM_TRAFFIC_TRANSITIONS (sizeof(traffic_transitions) / sizeof(traffic_transitions[0]))

// ========================
// HELPER FUNCTIONS
// ========================

int get_state_duration(sm_state_t state) {
    switch(state) {
        case TRAFFIC_RED:        return RED_DURATION_SEC;
        case TRAFFIC_RED_YELLOW: return RED_YELLOW_DURATION_SEC;
        case TRAFFIC_GREEN:      return GREEN_DURATION_SEC;
        case TRAFFIC_YELLOW:     return YELLOW_DURATION_SEC;
        default:                 return 1;
    }
}

void print_menu(void) {
    printf("\n=== Traffic Light Controller ===\n");
    printf("Commands:\n");
    printf("  [SPACE/ENTER] - Timer expired (normal flow)\n");
    printf("  e - Emergency stop\n");
    printf("  r - Reset system\n");
    printf("  s - Show status\n");
    printf("  l - Toggle logging\n");
    printf("  a - Automatic cylcle\n");
    printf("  q - Quit\n");
    printf("================================\n");
}

void simulate_automatic_cycle(state_machine_t *traffic_sm) {
    printf("\n Starting automatic traffic light simulation for 30 seconds...\n");
    printf("Press Ctrl+C to stop and return to manual mode.\n\n");
    
    time_t start_time = time(NULL);
    time_t last_transition = start_time;
    
    while ((time(NULL) - start_time) < 30) {  // Run for 30 seconds
        sm_state_t current_state;
        sm_get_current_state(traffic_sm, &current_state);
        
        int duration = get_state_duration(current_state);
        
        if ((time(NULL) - last_transition) >= duration) {
            printf("\n Timer expired - triggering transition...\n");
            sm_result_t result = sm_process_event(traffic_sm, EVENT_TIMER_EXPIRED);
            
            if (result == SM_SUCCESS) {
                last_transition = time(NULL);
            } else {
                printf("Transition failed: %d\n", result);
            }
        }
        
        sleep(1);  // Check every second
    }
    
    printf("\n Automatic simulation completed!\n");
}

// ========================
// MAIN FUNCTION
// ========================

int main(void) {
    // Create and initialize traffic light state machine
    state_machine_t traffic_sm;
    
    printf("Initializing Traffic Light State Machine...\n");
    
    sm_result_t result = sm_init(&traffic_sm, 
                                "TrafficLight", 
                                TRAFFIC_RED,
                                traffic_states, 
                                NUM_TRAFFIC_STATES,
                                traffic_transitions, 
                                NUM_TRAFFIC_TRANSITIONS);
    
    if (result != SM_SUCCESS) {
        printf("Failed to initialize traffic light: %d\n", result);
        return 1;
    }
    
    printf("Traffic Light initialized successfully!\n");
    
    // Enable logging by default
    sm_set_logging(&traffic_sm, true);
    
    // Show initial status
    sm_print_status(&traffic_sm);
    
    // Interactive loop
    char input;
    bool running = true;
    
    print_menu();
    
    while (running) {
        printf("\nCurrent state: %s\n", sm_get_state_name(&traffic_sm, traffic_sm.current_state));
        printf("Enter command: ");
        fflush(stdout);
        
        input = getchar();
        
        // Clear input buffer
        if (input != '\n') {
            while (getchar() != '\n');
        }
        
        switch (input) {
            case ' ':
            case '\n':
            case 't':
                printf("Processing timer expired event...\n");
                result = sm_process_event(&traffic_sm, EVENT_TIMER_EXPIRED);
                if (result != SM_SUCCESS) {
                    printf("No valid transition for timer event from current state\n");
                }
                break;
                
            case 'e':
            case 'E':
                printf("Processing emergency stop...\n");
                result = sm_process_event(&traffic_sm, EVENT_EMERGENCY_STOP);
                if (result != SM_SUCCESS) {
                    printf("No emergency transition from current state\n");
                }
                break;
                
            case 'r':
            case 'R':
                printf("Processing reset...\n");
                result = sm_process_event(&traffic_sm, EVENT_RESET);
                break;
                
            case 's':
            case 'S':
                sm_print_status(&traffic_sm);
                break;
                
            case 'l':
            case 'L':
                traffic_sm.logging_enabled = !traffic_sm.logging_enabled;
                printf("Logging %s\n", traffic_sm.logging_enabled ? "enabled" : "disabled");
                sm_set_logging(&traffic_sm, traffic_sm.logging_enabled);
                break;
                
            case 'a':
            case 'A':
                simulate_automatic_cycle(&traffic_sm);
                print_menu();
                break;
                
            case 'h':
            case 'H':
                print_menu();
                break;
                
            case 'q':
            case 'Q':
                printf("Shutting down traffic light system...\n");
                running = false;
                break;
                
            default:
                printf("Unknown command. Press 'h' for help.\n");
                break;
        }
    }
    
    // Final statistics
    uint32_t total_transitions, invalid_events;
    sm_get_stats(&traffic_sm, &total_transitions, &invalid_events);
    
    printf("\n Final Statistics:\n");
    printf("   Total transitions: %lu\n", (unsigned long)total_transitions);
    printf("   Invalid events: %lu\n", (unsigned long)invalid_events);
    
    printf("Traffic light system shut down successfully.\n");
    
    return 0;
}