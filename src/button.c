/*
 *  Button lib
 *  pyroesp
 */

#include "button.h"


void button_init(Button *b, uint8_t size){
    int8_t i;
    for (i = 0; i < size; i++){
        io_init(&b[i].btn, 0, 0, 0, 0);
        b[i].input = BUTTON_LOGIC_OFF;
        b[i].pressed_time = 0;
        b[i].status = BUTTON_OFF;
        b[i].to_super_speed = 0;
    }
    return;
}

void button_set_io(IO *pin, uint8_t *in, uint8_t bit){
    io_init(pin, 0, in, bit, 0);
    return;
}

/* Button State Machine

                BUTTON_OFF <---------------------------<--------------------------------<-----------+
                    |                                                                               |
                    v                                                                               |
                    |          0             SHORT                                                  |
              BUTTON_PRESSED -->-----+-->--+--------> BUTTON_SHORT_PRESS ->-+                       |
                    |                |     |                                |                       ^
                    v 1              v     v                                v                       |
                    |            0   |     |  LONG                          |                       |
             BUTTON_LONG_PRESS -->---+     +--------> BUTTON_LONG_PRESS -->-+                       |
                    |                |                                      |                       |
                    v 1              v                                      |                       |
                    |        0       |                                      v                       |
         +---> BUTTON_HOLD -->-------+                                      |                       |
         |          |                |                                      |                       |
         ^ 1        v 1              v                                      |                       |
         |          |            0   |                                      |                       ^
    +-->-+- BUTTON_SHORT_PRESS -->---+                                      |                       |
    |               |                |                                      v                       |
    ^ 1             v HOLD x 5       v                                      |                       |
    |               |              0 |                                      |                       |
    +---- BUTTON_HOLD_SUPER_SPEED ->-+---------------->---------------------+--> BUTTON_RELEASED ->-+

*/
void button_update(Button *b, uint16_t ms){
    b->input = (io_read_pin(&b->btn) == BUTTON_LOGIC_ON);               // get input
    if (b->input == 1){                                                 // if button pressed
        switch (b->status){                                             // check status
            case BUTTON_OFF:                                            // if button status is off
                b->to_super_speed = 0;
                b->pressed_time = 0;                                    // reset button timer
                b->status = BUTTON_PRESSED;                             // set status to pressed
                break;
            case BUTTON_PRESSED:                                        // if pressed
                b->pressed_time += ms;                                  // increase timer
                if (b->pressed_time >= BUTTON_LONG_TIME){               // if timer reached long press time
                    b->status = BUTTON_LONG_PRESS;                      // next status is long press
                }
                break;
            case BUTTON_LONG_PRESS:                                             // if pressed
                b->pressed_time += ms;                                          // increase timer
                if (b->pressed_time >= (BUTTON_LONG_TIME+BUTTON_HOLD_TIME)){    // if timer reached long press time
                    b->pressed_time = 0;                                        // reset button timer
                    b->status = BUTTON_HOLD;                                    // next status is hold
                }
                break;
            case BUTTON_HOLD:                                           // if hold
                b->pressed_time += ms;                                  // increase timer
                if (b->pressed_time >= BUTTON_HOLD_TIME){               // if timer reached hold time
                    b->pressed_time = 0;                                // reset timer
                    b->to_super_speed++;                                // increase counter to super speed hold
                    b->status = BUTTON_SHORT_PRESS;                     // next status is short press
                }
                break;
            case BUTTON_SHORT_PRESS:                                    // if short press (only possible during hold)
                if (b->to_super_speed >= BUTTON_HOLD_TO_SUPER_SPEED){   // check if super speed can be enabled
                    b->status = BUTTON_HOLD_SUPER_SPEED;                // next status is pressed hold super speed
                } else {
                    b->status = BUTTON_HOLD;                            // next status is pressed hold
                }
                break;
            case BUTTON_HOLD_SUPER_SPEED:                               // if hold super speed
                b->pressed_time += ms;                                  // increase timer
                if (b->pressed_time >= BUTTON_SUPER_SPEED_TIME){        // if timer reached super speed time
                    b->pressed_time = 0;                                // reset timer
                    b->status = BUTTON_SHORT_PRESS;                     // next status is short press
                }
                break;
            default:
                b->status = BUTTON_OFF;
                break;
        }
    } else if (b->input == 0){                                          // when button released
        switch (b->status){                                             // check status
            case BUTTON_PRESSED:                                        // if pressed
                if (b->pressed_time >= BUTTON_LONG_TIME){               // check long timeout
                    b->status = BUTTON_LONG_PRESS;                      // next status is long press
                } else if (b->pressed_time >= BUTTON_SHORT_TIME){       // else check short timeout
                    b->status = BUTTON_SHORT_PRESS;                     // next status is short press
                } else {
                    b->status = BUTTON_OFF;                             // if no pressed timer exceeded, turn button off
                }
                break;
            case BUTTON_HOLD:                                           // if status is hold, super speed, long or short press
            case BUTTON_HOLD_SUPER_SPEED:
            case BUTTON_LONG_PRESS:
            case BUTTON_SHORT_PRESS:
                b->status = BUTTON_RELEASED;                            // next status is released
                break;
            case BUTTON_RELEASED:                                       // if released
            default:
                b->status = BUTTON_OFF;                                 // next status is button off
                break;
        }
    }
    return;
}


uint8_t button_are_off(Button *b, uint8_t size){
    int8_t i;
    for (i = 0; i < size; i++){
        if (b[i].status != BUTTON_OFF){
            return 0;
        }
    }
    return 1;
}


uint8_t button_are_active(Button *b, uint8_t size){
    int8_t i;
    uint8_t active = 0;
    for (i = 0; i < size; i++){
        if (b[i].status != BUTTON_OFF){
            active++;
        }
    }
    return active;
}

void button_copy_state(Button *src, Button *dst, uint8_t size){
    int8_t i;
    for (i = 0; i < size; i++){
        dst[i].status = src[i].status;
    }
    return;
}
