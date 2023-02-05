#include "include/defaultInputEvents.h"
#include "include/inputEvents.h"

#include <stdio.h>

input_event_state_t state_PrimaryButton[MAX_PLAYER_COUNT];
input_event_state_t state_SecondaryButton[MAX_PLAYER_COUNT];
input_event_state_t state_CoPrimaryButton[MAX_PLAYER_COUNT];
input_event_state_t state_CoSecondaryButton[MAX_PLAYER_COUNT];
input_event_state_t state_HomeButton[MAX_PLAYER_COUNT];
input_event_state_t state_MenuButton[MAX_PLAYER_COUNT];
input_event_state_t state_ShoulderButton[MAX_PLAYER_COUNT];
input_event_state_t state_LeftButton[MAX_PLAYER_COUNT];
input_event_state_t state_UpButton[MAX_PLAYER_COUNT];
input_event_state_t state_RightButton[MAX_PLAYER_COUNT];
input_event_state_t state_DownButton[MAX_PLAYER_COUNT];
input_event_state_t state_Joystick[MAX_PLAYER_COUNT];
input_event_state_t state_RJoystick[MAX_PLAYER_COUNT];

void init_default_event_lib() {
    for (uint8_t i = 1; i <= MAX_PLAYER_COUNT; i++) {
        input_event_init(&state_PrimaryButton[i], false, 500, 100);
        input_event_init(&state_SecondaryButton[i], false, 500, 100);
        input_event_init(&state_CoPrimaryButton[i], false, 500, 100);
        input_event_init(&state_CoSecondaryButton[i], false, 500, 100);
        input_event_init(&state_HomeButton[i], false, 500, 100);
        input_event_init(&state_MenuButton[i], false, 500, 100);
        input_event_init(&state_ShoulderButton[i], false, 500, 100);
        input_event_init(&state_LeftButton[i], false, 500, 100);
        input_event_init(&state_UpButton[i], false, 500, 100);
        input_event_init(&state_RightButton[i], false, 500, 100);
        input_event_init(&state_DownButton[i], false, 500, 100);
        input_event_init(&state_Joystick[i], false, 500, 100);
        input_event_init(&state_RJoystick[i], false, 500, 100);
    }
}

bool gotPressed(input_event_e event, bool withRepeats) {
    return (event == INPUT_EVENT_PRESS || (withRepeats && event == INPUT_EVENT_REPEAT));
}

bool gotPrimaryButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_PrimaryButton[player_num], isPrimaryButtonPressed(player_num)), with_repeats);
}
bool gotSecondaryButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_SecondaryButton[player_num], isSecondaryButtonPressed(player_num)), with_repeats);
}
bool gotCoPrimaryButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_CoPrimaryButton[player_num], isCoPrimaryButtonPressed(player_num)), with_repeats);
}
bool gotCoSecondaryButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_CoSecondaryButton[player_num], isCoSecondaryButtonPressed(player_num)), with_repeats);
}
bool gotHomeButtonPressed(uint8_t player_num, bool with_repeats) {
    state_MenuButton[player_num].prev_state = true;
    return gotPressed(input_event_poll(&state_HomeButton[player_num], isHomeButtonPressed(player_num)), with_repeats);
}
bool gotMenuButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_MenuButton[player_num], isMenuButtonPressed(player_num)), with_repeats);
}
bool gotShoulderButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_ShoulderButton[player_num], isShoulderButtonPressed(player_num)), with_repeats);
}
bool gotLeftButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_LeftButton[player_num], getLRInput(player_num) < -60), with_repeats);
}
bool gotUpButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_UpButton[player_num], getUDInput(player_num) < -60), with_repeats);
}
bool gotRightButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_RightButton[player_num], getLRInput(player_num) > 60), with_repeats);
}
bool gotDownButtonPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_DownButton[player_num], getUDInput(player_num) > 60), with_repeats);
}
bool gotJoystickPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_Joystick[player_num], isJoystickPressed(player_num)), with_repeats);
}
bool gotRJoystickPressed(uint8_t player_num, bool with_repeats) {
    return gotPressed(input_event_poll(&state_RJoystick[player_num], isRJoystickPressed(player_num)), with_repeats);
}