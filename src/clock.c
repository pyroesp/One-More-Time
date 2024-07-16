/*
 *  Clock lib
 *  pyroesp
 */

#include "clock.h"

void clock_init(Clock *c){
    time_init(&c->time);
    date_init(&c->date);

    alarm_init(c->alarms, ALARM_SIZE);
    c->alarm_index = 0;

    vfd_init(&c->vfd);
    button_init(c->buttons, BUTTON_SIZE);
    button_init(c->prev_buttons, BUTTON_SIZE);

    c->mode = CLOCK_MODE_TIME;
    c->change_mode_time = 0;
    c->blink = CLOCK_BLINK_ON;
    c->blink_time = 0;

    mp3_init(&c->mp3);
    return;
}

void clock_blink(Clock *c, uint16_t ms){
    c->blink_time += ms;
    if (c->blink == CLOCK_BLINK_OFF){                   // if blink state is off
        if (c->blink_time >= CLOCK_BLINK_TIME_OFF){     // check blink off timeout
            c->blink_time = 0;                          // reset timer
            c->blink = CLOCK_BLINK_ON;                  // go to blink on state
        }
    } else if (c->blink == CLOCK_BLINK_ON){             // if blink state is on
        if (c->blink_time >= CLOCK_BLINK_TIME_ON){      // check blink off timeout
            c->blink_time = 0;                          // reset timer
            c->blink = CLOCK_BLINK_OFF;                 // go to blink off state
        }
    }
    return;
}

void clock_mode_change(Clock *c, uint16_t ms){
    if (c->mode == CLOCK_MODE_TIME){                                            // in clock mode time
        if (button_are_active(c->buttons, BUTTON_SIZE) > 1){                    // check if multiple buttons being pressed
            if (c->buttons[BUTTON_HOUR].status == BUTTON_LONG_PRESS             // check for hour & minute buttons
                    && c->buttons[BUTTON_MINUTE].status == BUTTON_LONG_PRESS){
                // #TODO: Check if ESP32 connected & synced
                c->mode = CLOCK_MODE_DATE;                                      // go to date mode
            }
            // #TODO: Show ESP32 IP address if ESP32 connected
        } else {                                                                // else if only 1 button
            if (c->buttons[BUTTON_HOUR].status == BUTTON_LONG_PRESS){           // check hour button
                c->blink_time = 0;                                              // reset blink timer
                c->mode = CLOCK_MODE_CHANGE_HOUR;                               // go to change hour mode
            } else if (c->buttons[BUTTON_MINUTE].status == BUTTON_LONG_PRESS){  // check minute button
                c->blink_time = 0;                                              // reset blink timer
                c->mode = CLOCK_MODE_CHANGE_MINUTE;                             // go to change minute mode
            } else if (c->buttons[BUTTON_ALARM].status == BUTTON_LONG_PRESS){   // check alarm button
                c->mode = CLOCK_MODE_CHANGE_ALARM;                              // go to change alarm mode
            } else if (c->buttons[BUTTON_SNOOZE].status == BUTTON_LONG_PRESS){  // check snooze button
                c->mode = CLOCK_MODE_CHANGE_DAY_OF_WEEK;                        // go to change day of week mode
            }
        }
    }

    // check if buttons are off for 5s and if so return to display time mode
    if (button_are_off(c->buttons, BUTTON_SIZE)){
        c->change_mode_time += ms;
        if (c->change_mode_time >= CLOCK_EXIT_CHANGE_MODE_TIME){
            c->mode = CLOCK_MODE_TIME;
            c->change_mode_time = 0;
        }
    } else {
        c->change_mode_time = 0;
    }
    return;
}

void clock_mode(Clock *c, uint16_t ms){ // execution of different modes
    switch (c->mode){                                                           // check clock mode
        case CLOCK_MODE_TIME:                                                   // display time mode
            // display "HHMMSS"
            vfd_set_time(&c->vfd, &c->time);                                    // set time to vfd display
            alarm_update(&c->alarms[c->date.day_of_week], &c->time, ms,         // check alarm of the day
                c->buttons[BUTTON_SNOOZE].status == BUTTON_SHORT_PRESS,         // boolean calc for snoozed
                c->buttons[BUTTON_ALARM].status == BUTTON_LONG_PRESS);          // boolean calc for alarm off
            alarm_audio(&c->alarms[c->date.day_of_week], &c->mp3);              // play/pause/stop alarm
            break;
        case CLOCK_MODE_CHANGE_HOUR:                                            // change hour
            // display "HHMMSS"
            vfd_set_time(&c->vfd, &c->time);                                    // show time
            if (c->blink == CLOCK_BLINK_OFF){                                   // check for blink state off
                c->vfd.display[VFD_DISPLAY_HOUR_UNIT] = VFD_SEGMENT_OFF;        // blink off hours
                c->vfd.display[VFD_DISPLAY_HOUR_TENS] = VFD_SEGMENT_OFF;
            }
            clock_blink(c, ms);                                                 // update blink status

            if (c->buttons[BUTTON_HOUR].status == BUTTON_SHORT_PRESS){          // increase hour on short press
                time_inc_hour(&c->time);
            }
            break;
        case CLOCK_MODE_CHANGE_MINUTE:                                          // change minute
            // display "HHMMSS"
            vfd_set_time(&c->vfd, &c->time);                                    // show time
            if (c->blink == CLOCK_BLINK_OFF){                                   // check for blink state off
                c->vfd.display[VFD_DISPLAY_MINUTE_UNIT] = VFD_SEGMENT_OFF;      // blink off minutes
                c->vfd.display[VFD_DISPLAY_MINUTE_TENS] = VFD_SEGMENT_OFF;
            }
            clock_blink(c, ms);                                                 // update blink status

            if (c->buttons[BUTTON_MINUTE].status == BUTTON_SHORT_PRESS){        // increase minute on short press
                time_inc_minute(&c->time);
            }
            break;
        case CLOCK_MODE_CHANGE_ALARM:                                           // change alarm
            // display "d1.HHMM"
            vfd_set_alarm(&c->vfd, &c->alarms[c->alarm_index].t,                // show change alarm mode
                    c->alarm_index + 1, c->alarms[c->alarm_index].enabled);


            if (c->buttons[BUTTON_ALARM].status == BUTTON_RELEASED              // toggle alarm on and off with short press and release
                    && c->prev_buttons[BUTTON_ALARM].status == BUTTON_SHORT_PRESS){
                if (c->alarms[c->alarm_index].enabled == ALARM_OFF){
                    c->alarms[c->alarm_index].enabled = ALARM_ON;
                } else {
                    c->alarms[c->alarm_index].enabled = ALARM_OFF;
                }
            } else if (c->buttons[BUTTON_SNOOZE].status == BUTTON_RELEASED      // change selected alarm with snooze short press and release
                    && c->prev_buttons[BUTTON_SNOOZE].status == BUTTON_SHORT_PRESS){
                c->alarm_index++;
                if (c->alarm_index >= ALARM_SIZE){
                    c->alarm_index = 0;
                }
            }

            if (c->buttons[BUTTON_HOUR].status == BUTTON_SHORT_PRESS){          // change hour of alarm with short press
                time_inc_hour(&c->alarms[c->alarm_index].t);
            } else if (c->buttons[BUTTON_MINUTE].status == BUTTON_SHORT_PRESS){ // change minute of alarm with short press
                time_inc_minute(&c->alarms[c->alarm_index].t);
            }
            break;
        case CLOCK_MODE_CHANGE_DAY_OF_WEEK:                                     // change day of week
            // display "do X  "
            vfd_set_day_of_week(&c->vfd, c->date.day_of_week + 1);              // show change day of week mode


            if (c->buttons[BUTTON_SNOOZE].status == BUTTON_RELEASED             // change day of week with snooze short press and release
                    && c->prev_buttons[BUTTON_SNOOZE].status == BUTTON_SHORT_PRESS){
                c->date.day_of_week++;
                if (c->date.day_of_week >= 7){
                    c->date.day_of_week = 0;
                }
            }
            break;
        case CLOCK_MODE_DATE:                                                   // scroll date
            vfd_set_date(&c->vfd, &c->date, c->change_mode_time);               // show scrolling date
            break;
        default:
            break;
    }
    return;
}

void clock_update(Clock *c, uint16_t ms){ // update clock stuff (maybe around ~20x/s to ~50x/s)
    int8_t i;
    for (i = 0; i < BUTTON_SIZE; i++){
        button_update(&c->buttons[i], ms);                          // update all buttons state
    }

    time_ms(&c->time, ms);                                          // update millisecond

    clock_mode_change(c, ms);                                       // check if clock mode needs to be changed
    clock_mode(c, ms);                                              // execute code for the selected mode

    button_copy_state(c->buttons, c->prev_buttons, BUTTON_SIZE);    // copy current button state to previous
}
