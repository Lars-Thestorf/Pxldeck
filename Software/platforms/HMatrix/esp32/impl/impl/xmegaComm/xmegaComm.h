#ifndef _LALEMA_XMEGACOMM_H_
#define _LALEMA_XMEGACOMM_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define buttons_t uint16_t
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_X (1 << 2)
#define BUTTON_Y (1 << 3)
#define BUTTON_L (1 << 4)
#define BUTTON_R (1 << 5)
#define BUTTON_SELECT (1 << 6)
#define BUTTON_START (1 << 7)
#define BUTTON_DL (1 << 8)
#define BUTTON_DU (1 << 9)
#define BUTTON_DR (1 << 10)
#define BUTTON_DD (1 << 11)
#define BUTTON_JL (1 << 12)
#define BUTTON_JR (1 << 13)
#define BUTTON_CRG (1 << 14)
#define BUTTON_PWR (1 << 15)

void SetupXmegaComm(void);
void XmegaCommWorker(void);

bool isXmegaCommEstablished(void);

buttons_t xmegaGetPressedButtons(void);

uint8_t xmegaGetJoystickXAxis(void);
uint8_t xmegaGetJoystickYAxis(void);
uint8_t xmegaGetJoystickRXAxis(void);
uint8_t xmegaGetJoystickRYAxis(void);

uint16_t xmegaGetBatteryLevel(void);

#ifdef __cplusplus
}
#endif

#endif
