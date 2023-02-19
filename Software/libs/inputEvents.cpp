#include "include/inputEvents.h"
#include <HLM_time.h>

void input_event_init(input_event_state_t *state, bool initial_state, uint64_t repeat_delay, uint64_t repeat_interval) {
    state->prev_state = initial_state;
    state->press_timestamp = 0;
    state->repating = false;
    state->repeat_delay = repeat_delay;
    state->repeat_interval = repeat_interval;
}
input_event_e input_event_poll(input_event_state_t *state, bool current_state) {
    uint64_t timestamp = get_ms_since_boot();
    if (current_state && !state->prev_state) {
        state->prev_state = current_state;
        state->press_timestamp = timestamp;
        state->repating = false;
        return INPUT_EVENT_PRESS;
    }
    if (!current_state && state->prev_state) {
        state->prev_state = current_state;
        return INPUT_EVENT_RELEASE;
    }
    if (current_state && !state->repating && timestamp - state->press_timestamp >= state->repeat_delay) {
        state->repating = true;
        state->press_timestamp = timestamp;
        return INPUT_EVENT_REPEAT;
    }
    if (current_state && state->repating && timestamp - state->press_timestamp >= state->repeat_interval) {
        state->press_timestamp = timestamp;
        return INPUT_EVENT_REPEAT;
    }
    return INPUT_EVENT_NONE;
}