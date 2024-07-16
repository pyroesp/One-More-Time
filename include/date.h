/*
 *  Date lib
 *  pyroesp
 */

#ifndef __DATE_H
#define	__DATE_H

#include <stdint.h>

// Date struct
typedef struct{
    uint8_t year;           // year
    uint8_t month;          // month
    uint8_t day;            // day
    uint8_t day_of_week;    // day of week
}Date;

// Date init
void date_init(Date *d);
// Date update on midnight
void date_update(Date *d, uint8_t is_midnight);


#endif
