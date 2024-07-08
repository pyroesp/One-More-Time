/*
 *  Time lib
 *  pyroesp
 */

#ifndef __TIME_H
#define	__TIME_H

#include <stdint.h>

// Time struct
typedef struct{
    uint8_t hour;           // hours
    uint8_t minute;         // minutes
    uint8_t second;         // seconds
    uint16_t millisecond;   // ms
}Time;

// Time init
void time_init(Time *t);
// Add ms to time
void time_ms(Time *t, uint16_t ms);
// Increment hour
uint8_t time_inc_hour(Time *t);
// Increment minute
uint8_t time_inc_minute(Time *t);
// Increment second
uint8_t time_inc_second(Time *t);
// Update time
uint8_t time_update(Time *t);
// Check if time is midnight
uint8_t time_is_midnight(Time *t);

#endif
