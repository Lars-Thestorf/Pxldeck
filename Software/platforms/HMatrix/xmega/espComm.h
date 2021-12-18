

#ifndef _LALEMA_ESPCOMM_H_
#define _LALEMA_ESPCOMM_H_

#include <stdbool.h>
#include <stdint.h>
#include "buttonMatrix.h"

#define BUTTON_JL (1 << 12)
#define BUTTON_JR (1 << 13)

#define BUTTON_CRG (1 << 14)
#define BUTTON_PWR (1 << 15)

void SetupComm(void);
void SendDeviceStatus(void);

#endif

