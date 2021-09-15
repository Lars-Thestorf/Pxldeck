#ifndef _LALEMA_ADCMULTIPIN_H_
#define _LALEMA_ADCMULTIPIN_H_

#include <stdint.h>

void SetupAdcMultipin(void);

uint16_t getAnalogValue(uint8_t pin);

#endif

