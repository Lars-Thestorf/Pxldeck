#ifndef LALEMA_INPUT_EVENTS_LIB_H
#define LALEMA_INPUT_EVENTS_LIB_H 1

#include <stdint.h>

typedef struct input_event_state_t {
    bool prev_state;
    bool repating;
    uint64_t press_timestamp;
    uint64_t repeat_delay;
    uint64_t repeat_interval;
} input_event_state_t;

typedef enum input_event_e {
    INPUT_EVENT_NONE,
    INPUT_EVENT_PRESS,
    INPUT_EVENT_RELEASE,
    INPUT_EVENT_REPEAT,
} input_event_e;

#define INPUT_EVENT_NO_REPEAT 1<<63

void input_event_init(input_event_state_t *state, bool initial_state, uint64_t repeat_delay, uint64_t repeat_interval);
input_event_e input_event_poll(input_event_state_t *state, bool current_state);

#endif