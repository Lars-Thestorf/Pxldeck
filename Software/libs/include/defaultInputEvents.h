#ifndef LALEMA_DEFAULT_INPUT_EVENTS_LIB_H
#define LALEMA_DEFAULT_INPUT_EVENTS_LIB_H 1

#include <HLM_playerInput.h>

void init_default_event_lib();

bool gotPrimaryButtonPressed(uint8_t player_num, bool with_repeats);
bool gotSecondaryButtonPressed(uint8_t player_num, bool with_repeats);
bool gotCoPrimaryButtonPressed(uint8_t player_num, bool with_repeats);
bool gotCoSecondaryButtonPressed(uint8_t player_num, bool with_repeats);
bool gotHomeButtonPressed(uint8_t player_num, bool with_repeats);
bool gotMenuButtonPressed(uint8_t player_num, bool with_repeats);
bool gotShoulderButtonPressed(uint8_t player_num, bool with_repeats);
bool gotLeftButtonPressed(uint8_t player_num, bool with_repeats);
bool gotUpButtonPressed(uint8_t player_num, bool with_repeats);
bool gotRightButtonPressed(uint8_t player_num, bool with_repeats);
bool gotDownButtonPressed(uint8_t player_num, bool with_repeats);
bool gotJoystickPressed(uint8_t player_num, bool with_repeats);
bool gotRJoystickPressed(uint8_t player_num, bool with_repeats);

#endif