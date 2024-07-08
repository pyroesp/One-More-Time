/*
 *  Alarm lib
 *  pyroesp
 */

#ifndef __ALARM_H
#define	__ALARM_H

#include <stdint.h>
#include "time.h"

#define ALARM_SNOOZE_TIMEOUT 300 // in seconds
#define ALARM_OFF 0
#define ALARM_ON 1

#define ALARM_SIZE 7

// Alarm state machine
typedef enum{
    ALARM_INACTIVE = 0,
    ALARM_STATE_OFF,
    ALARM_ACTIVE,
    ALARM_PLAYING,
    ALARM_SNOOZED,
    ALARM_SNOOZING
}Alarm_State;

// Alarm structure
typedef struct{
    Time t;                     // time struct
    uint8_t enabled;            // alarm enabled or disabled
    uint16_t snooze_time_s;     // snooze time in s
    uint16_t snooze_time_ms;    // snooze time in ms
    Alarm_State status;         // alarm state
}Alarm;

// Alarm struct init
void alarm_init(Alarm *a, uint8_t size);
// Alarm update
void alarm_update(Alarm *a, Time *t, uint16_t ms, uint8_t snoozed, uint8_t alarm_off);
// Alarm play/pause/stop audio
void alarm_audio(Alarm *a);

#endif

