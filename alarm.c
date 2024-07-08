/*
 *  Alarm lib
 *  pyroesp
 */

#include "alarm.h"


void alarm_init(Alarm *a, uint8_t size){
    int8_t i;
    for (i = 0; i < size; i++){
        time_init(&a[i].t);
        a[i].enabled = ALARM_OFF;
        a[i].snooze_time_s = 0;
        a[i].snooze_time_ms = 0;
        a[i].status = ALARM_INACTIVE;
    }
    return;
}

/* Alarm State Machine
    
    +-------------------------------------------------<------------------------------------------------------------+
    +                                   +----------------------------<----------------------------+                +
    +-> ALARM_INACTIVE -> ALARM_ACTIVE -+-> ALARM_PLAYING -+-> ALARM_SNOOZED -+-> ALARM_SNOOZING -+-+              +
                                                           +----------------------------------------+-> ALARM_OFF -+
*/
void alarm_update(Alarm *a, Time *t, uint16_t ms, uint8_t snoozed, uint8_t alarm_off){
    if (a->enabled){                                            // if alarm enabled
        switch(a->status){
            case ALARM_INACTIVE:                                // if alarm inactive
                if (t->hour == a->t.hour &&                     // check current time vs alarm time
                    t->minute == a->t.minute && 
                    t->second == a->t.second){
                    a->snooze_time_ms = 0;                      // reset snooze timeout
                    a->snooze_time_s = 0;                       // reset snooze timeout
                    a->status = ALARM_ACTIVE;                   // set alarm status to active
                }
                break;
            case ALARM_ACTIVE:                                  // if alarm status is active
                a->status = ALARM_PLAYING;                      // next status is playing
                break;
            case ALARM_PLAYING:                                 // if alarm status is playing
                if (snoozed){                                   // check if snooze button pressed
                    a->status = ALARM_SNOOZED;                  // next status is snoozed
                } else if (alarm_off){                          // check if alarm off is pressed
                    a->status = ALARM_STATE_OFF;                // next status is alarm off
                }
                break;
            case ALARM_SNOOZED:                                 // if alarm status is snoozed
                a->status = ALARM_SNOOZING;                     // next status is snoozing
                a->snooze_time_s = 0;                           // reset timeout
                a->snooze_time_ms = 0;                          // reset snooze timeout
                break;
            case ALARM_SNOOZING:                                // if snoozing
                a->snooze_time_ms += ms;                        // increase timer
                
                if (a->snooze_time_s >= ALARM_SNOOZE_TIMEOUT){  // check snooze timeout
                    a->snooze_time_s = 0;                       // reset timeout
                    a->snooze_time_ms = 0;
                    a->status = ALARM_ACTIVE;                   // next status is active
                } else if (a->snooze_time_ms > 1000){
                    a->snooze_time_s++;
                    a->snooze_time_ms -= 1000;
                }
                
                if (alarm_off){                                 // check if alarm off is pressed during snoozing
                    a->status = ALARM_STATE_OFF;                // next status is alarm off
                }
                break;
            case ALARM_STATE_OFF:                               // if alarm off
            default:
                a->status = ALARM_INACTIVE;                     // next status is inactive
                break;
        }
    }
    return;
}

void alarm_audio(Alarm *a){     // Play audio function
    switch(a->status){
        case ALARM_ACTIVE:                                  // if alarm status is active
            // TODO: MP3 play music (on repeat?)            // start audio
            break;
        case ALARM_SNOOZED:                                 // if alarm status is snoozed
            // TODO: MP3 pause                              // pause audio
            break;
        case ALARM_STATE_OFF:                               // if alarm off
            // TODO: MP3 stop                               // stop audio
            break;
        default:
            break;
    }
    return;
}
