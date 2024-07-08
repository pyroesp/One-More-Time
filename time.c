/*
 *  Time lib
 *  pyroesp
 */

#include "time.h"


void time_init(Time *t){
    t->hour = 0;
    t->minute = 0;
    t->second = 0;
    t->millisecond = 0;
    return;
}

void time_ms(Time *t, uint16_t ms){
    t->millisecond += ms;
    return;
}

uint8_t time_inc_hour(Time *t){
    t->hour++;
    if (t->hour >= 24){
        t->hour = 0;
        return 1;
    }
    return 0;
}

uint8_t time_inc_minute(Time *t){
    t->minute++;
    if (t->minute >= 60){
        t->minute = 0;
        return 1;
    }
    return 0;
}

uint8_t time_inc_second(Time *t){
    t->second++;
    if (t->second >= 60){
        t->second = 0;
        return 1;
    }
    return 0;
}

uint8_t time_update(Time *t){
    if (time_inc_second(t)){
        if (time_inc_minute(t)){
            if (time_inc_hour(t)){
                return 1;
            }
        }
    }
    return 0;
}

uint8_t time_is_midnight(Time *t){
    if (t->hour == 0 && t->minute == 0 && t->second == 0){
        return 1;
    }
    return 0;
}
