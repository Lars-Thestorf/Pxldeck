#ifndef _LALEMA_SETUP_H_
#define _LALEMA_SETUP_H_

/* Helpers */
#define PASTE3_INNER(x,y,z) x ## y ## z
#define PASTE3(x,y,z) PASTE3_INNER(x,y,z)

/* Pins */
#define PIN_BOOT_reg PORTC
#define PIN_BOOT_bit 0
#define PIN_BOOT_bm (1 << PIN_BOOT_bit)
#define PIN_EN_reg PORTC
#define PIN_EN_bit 1
#define PIN_EN_bm (1 << PIN_EN_bit)

#define PIN_KEEPALIVE_reg PORTD
#define PIN_KEEPALIVE_bit 3
#define PIN_KEEPALIVE_bm (1 << PIN_KEEPALIVE_bit)
#define PIN_PWRBTN_reg PORTD
#define PIN_PWRBTN_bit 4
#define PIN_PWRBTN_bm (1 << PIN_PWRBTN_bit)

#define PIN_BTNMATRIX_A_reg PORTD
#define PIN_BTNMATRIX_A_bit 2
#define PIN_BTNMATRIX_A_bm (1 << PIN_BTNMATRIX_A_bit)
#define PIN_BTNMATRIX_B_reg PORTD
#define PIN_BTNMATRIX_B_bit 1
#define PIN_BTNMATRIX_B_bm (1 << PIN_BTNMATRIX_B_bit)
#define PIN_BTNMATRIX_C_reg PORTD
#define PIN_BTNMATRIX_C_bit 0
#define PIN_BTNMATRIX_C_bm (1 << PIN_BTNMATRIX_C_bit)
#define PIN_BTNMATRIX_1_reg PORTE
#define PIN_BTNMATRIX_1_bit 1
#define PIN_BTNMATRIX_1_bm (1 << PIN_BTNMATRIX_1_bit)
#define PIN_BTNMATRIX_2_reg PORTE
#define PIN_BTNMATRIX_2_bit 0
#define PIN_BTNMATRIX_2_bm (1 << PIN_BTNMATRIX_2_bit)
#define PIN_BTNMATRIX_3_reg PORTB
#define PIN_BTNMATRIX_3_bit 2
#define PIN_BTNMATRIX_3_bm (1 << PIN_BTNMATRIX_3_bit)
#define PIN_BTNMATRIX_4_reg PORTB
#define PIN_BTNMATRIX_4_bit 3
#define PIN_BTNMATRIX_4_bm (1 << PIN_BTNMATRIX_4_bit)

#define PIN_BTN_JL_reg PORTB
#define PIN_BTN_JL_bit 0
#define PIN_BTN_JL_bm (1 << PIN_BTN_JL_bit)
#define PIN_BTN_JR_reg PORTE
#define PIN_BTN_JR_bit 2
#define PIN_BTN_JR_bm (1 << PIN_BTN_JR_bit)

#endif
