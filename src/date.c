/*
 *  Date lib
 *  pyroesp
 */

#include "date.h"


void date_init(Date *d){
    d->year = 24;
    d->month = 1;
    d->day = 1;
    d->day_of_week = 0;
    return;
}

void date_update(Date *d, uint8_t is_midnight){
    if (is_midnight){
        d->day_of_week++;
        if (d->day_of_week >= 7){
            d->day_of_week = 0;
        }
    }
    return;
}
