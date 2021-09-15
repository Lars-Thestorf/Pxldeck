#ifndef _LALEMA_BUTTONMATRIX_H_
#define _LALEMA_BUTTONMATRIX_H_

#include <stdbool.h>
#include <stdint.h>

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

void SetupButtonMatrix(void);
void ButtonMatrixWorker(void);

buttons_t getPressedButtons(void);

#endif

