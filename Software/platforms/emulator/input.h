#ifndef HLM_EMU_INPUT_H_
#define HLM_EMU_INPUT_H_

#include <stdint.h>
#include <stdbool.h>

#define BUTTON_PRIMARY 1
#define BUTTON_SECONDARY 2
#define BUTTON_COPRIMARY 3
#define BUTTON_COSECONDARY 4
#define BUTTON_LSHOULDER 5
#define BUTTON_RSHOULDER 6
#define BUTTON_LMENU 7
#define BUTTON_RMENU 8
#define BUTTON_DU 9
#define BUTTON_DD 10
#define BUTTON_DL 11
#define BUTTON_DR 12
#define BUTTON_HOME 13

void setButtonPressed(uint8_t button_idx, bool pressed);
bool getButtonPressed(uint8_t button_idx);

#endif


