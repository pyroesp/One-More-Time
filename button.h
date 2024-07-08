/*
 *  Button lib
 *  pyroesp
 */

#ifndef __BUTTON_H
#define	__BUTTON_H


#include <stdint.h>
#include "io.h"

// Signals
#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1

// Button press timeouts (ms)
#define BUTTON_SHORT_TIME 120
#define BUTTON_LONG_TIME 600
#define BUTTON_HOLD_TIME 350
#define BUTTON_SUPER_SPEED_TIME 90
// Hold counter to super speed (5 x BUTTON_HOLD_TIME)
#define BUTTON_HOLD_TO_SUPER_SPEED 5
// Button logic
#define BUTTON_LOGIC_ON ACTIVE_LOW
#define BUTTON_LOGIC_OFF ACTIVE_HIGH

// Button position
#define BUTTON_ALARM 0
#define BUTTON_HOUR 1
#define BUTTON_MINUTE 2
#define BUTTON_SNOOZE 3
// Button size
#define BUTTON_SIZE 4

// Button state machine
typedef enum{
    BUTTON_OFF = 0,
    BUTTON_RELEASED,
    BUTTON_PRESSED,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS,
    BUTTON_HOLD,
    BUTTON_HOLD_SUPER_SPEED
}Button_State;

// Button struct
typedef struct{
    IO btn;                     // I/O
    uint8_t input;              // Pressed=1, Released=0
    uint16_t pressed_time;      // in ms
    Button_State status;        // button state
    uint8_t to_super_speed;     // counter to super speed
}Button;

// Button init
void button_init(Button *b, uint8_t size);
// Button set io
void button_set_io(IO *pin, uint8_t *in, uint8_t bit);

// Button update
void button_update(Button *b, uint16_t ms);
// Check if all buttons off
uint8_t button_are_off(Button *b, uint8_t size);
// Check if multiple buttons on
uint8_t button_are_active(Button *b, uint8_t size);
// Button copy state
void button_copy_state(Button *src, Button *dst, uint8_t size);

#endif
