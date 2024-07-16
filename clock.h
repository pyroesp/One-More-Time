/*
 *  Clock lib
 *  pyroesp
 */

#ifndef __CLOCK_H
#define	__CLOCK_H

#include <stdint.h>
#include "time.h"
#include "date.h"
#include "alarm.h"
#include "button.h"
#include "VFD.h"
#include "mp3.h"

// Clock mode change exit timeout
#define CLOCK_EXIT_CHANGE_MODE_TIME 4000    // ms
// Clock blinking on/off timeout
#define CLOCK_BLINK_TIME_OFF 200            // ms
#define CLOCK_BLINK_TIME_ON 400             // ms

// Clock mode state machine
typedef enum{
    CLOCK_MODE_OFF = 0,
    CLOCK_MODE_TIME,                // Display time
    CLOCK_MODE_CHANGE_HOUR,         // Change hour
    CLOCK_MODE_CHANGE_MINUTE,       // Change minute
    CLOCK_MODE_CHANGE_ALARM,        // Change alarms
    CLOCK_MODE_CHANGE_DAY_OF_WEEK,  // Change day of week
    CLOCK_MODE_IP,                  // Show ESP32 IP (only if ESP32 present)
    CLOCK_MODE_DATE                 // Scroll date (only if ESP32 present and synced)
}Clock_Mode;

// Blink state
typedef enum{
    CLOCK_BLINK_OFF = 0,
    CLOCK_BLINK_ON
}Clock_Blink;

// Clock struct
typedef struct{
    Time time;                          // Time
    Date date;                          // Date

    Alarm alarms[ALARM_SIZE];           // Alarms
    uint8_t alarm_index;                // Alarm index

    VFD vfd;                            // VFD display
    Button buttons[BUTTON_SIZE];        // Buttons (current state)
    Button prev_buttons[BUTTON_SIZE];   // Buttons (previous state)
    IO led;                             // LED

    Clock_Mode mode;                    // Clock mode
    uint16_t change_mode_time;          // Change mode timeout

    Clock_Blink blink;                  // Blink state
    uint16_t blink_time;                // Blink timeout
    
    Mp3 mp3;                            // mp3 usart for alarms
}Clock;

// Clock init
void clock_init(Clock *c);
// Clock blink on/off
void clock_blink(Clock *c, uint16_t ms);
// Clock mode change with buttons
void clock_mode_change(Clock *c, uint16_t ms);
// Clock mode state machine
void clock_mode(Clock *c, uint16_t ms);
// Clock update buttons, time, alarms, ...
void clock_update(Clock *c, uint16_t ms);


#endif
