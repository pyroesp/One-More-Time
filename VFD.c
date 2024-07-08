/*
 *  VFD lib
 *  pyroesp
 */

#include "VFD.h"

/*
    ---a---
    |     |
    b     c
    |     |
    ---d---
    |     |
    e     f
    |     |
    ---g---
            .h

    0b hgfe dcba
*/
volatile uint8_t vfd_font[VFD_FONT_SIZE] = {
    //0bhgfedcba
    0x77,           // 0
    0x24,           // 1
    0x5D,           // 2
    0x6D,           // 3
    0x2E,           // 4
    0x6B,           // 5
    0x7B,           // 6
    0x25,           // 7
    0x7F,           // 8
    0x6F,           // 9
    0x3F,           // A
    0x7A,           // b
    0x53,           // C
    0x7C,           // d
    0x5C,           // E
    0x1C,           // F
    0,0,0,0,0,0,0,  // g,h,i,j,k,l,m
    0x38,           // n
    0x78,           // o
    0,0,0,0,0,0,0,  // p,q,r,s,t,u,v
    0,0,0,0         // w,x,y,z
};

void vfd_init(VFD *v){
    int8_t i;
    v->index = 0;
    io_init(&v->segment, 0, 0, 0, 0);
    io_init(&v->grid, 0, 0, 0, 0);
    for (i = 0; i < VFD_DISPLAY_SIZE; i++){
        v->display[i] = 0;
    }
    return;
}

// Convert value to 7 segment font
uint8_t vfd_font_map(uint8_t value){
    uint8_t r;
    if (value >= 0 && value <= 9){
        r = vfd_font[value];
    } else if (value >= '0' && value <= '9'){
        r = vfd_font[value - '0'];
    } else if (value >= 'a' && value <= 'o'){
        r = vfd_font[value - 'a' + 0x0A];
    } else if (value >= 'A' && value <= 'N'){
        r = vfd_font[value - 'A' + 0x0A];
    } else if (value == 'O'){
        r = vfd_font[0];
    } else {
        r = 0;
    }
    return r;
}

void vfd_set_io(IO *port, uint8_t *out, uint8_t bit, uint8_t shift){
    io_init(port, out, 0, bit, shift);
    return;
}

void vfd_set_dot(uint8_t *display, uint8_t dot){
    if (dot){
        (*display) |= (1 << VFD_BIT_DOT);
    } else {
        (*display) &= ~(1 << VFD_BIT_DOT);
    }
    return;
}

void vfd_write_io(VFD *v, uint8_t segment, uint8_t grid){
    io_write_port(&v->segment, segment);
    io_write_port(&v->grid, grid);
    return;
}

void vfd_write_all(VFD *v){
    vfd_write_io(v, v->display[v->index], ~(1 << v->index));
    v->index++;
    if (v->index >= VFD_DISPLAY_SIZE){
        v->index = 0;
    }
    return;
}

// Write "HHMMSS"
void vfd_set_time(VFD *v, Time *t){
    v->display[VFD_DISPLAY_SECOND_UNIT] = vfd_font_map(t->second % 10);
    v->display[VFD_DISPLAY_SECOND_TENS] = vfd_font_map(t->second / 10);
    v->display[VFD_DISPLAY_MINUTE_UNIT] = vfd_font_map(t->minute % 10);
    v->display[VFD_DISPLAY_MINUTE_TENS] = vfd_font_map(t->minute / 10);
    v->display[VFD_DISPLAY_HOUR_UNIT] = vfd_font_map(t->hour % 10);
    v->display[VFD_DISPLAY_HOUR_TENS] = vfd_font_map(t->hour / 10);
    return;
}

// Write "dX HH:MM", with X being the alarm day of week
void vfd_set_alarm(VFD *v, Time *t, uint8_t alarm, uint8_t enabled){
    v->display[VFD_DISPLAY_HOUR_TENS] = vfd_font_map('d');
    vfd_set_dot(&v->display[VFD_DISPLAY_HOUR_TENS], enabled);
    v->display[VFD_DISPLAY_HOUR_UNIT] = vfd_font_map(alarm);
    v->display[VFD_DISPLAY_MINUTE_TENS] = vfd_font_map(t->hour / 10);
    v->display[VFD_DISPLAY_MINUTE_UNIT] = vfd_font_map(t->hour % 10);
    v->display[VFD_DISPLAY_SECOND_TENS] = vfd_font_map(t->minute / 10);
    v->display[VFD_DISPLAY_SECOND_UNIT] = vfd_font_map(t->minute % 10);
    return;
}

// Write "do X  ", with X being the day of week
void vfd_set_day_of_week(VFD *v, uint8_t day_of_week){
    v->display[VFD_DISPLAY_HOUR_TENS] = vfd_font_map('d');
    v->display[VFD_DISPLAY_HOUR_UNIT] = vfd_font_map('o');
    v->display[VFD_DISPLAY_MINUTE_TENS] = 0;
    v->display[VFD_DISPLAY_MINUTE_UNIT] = vfd_font_map(day_of_week);
    v->display[VFD_DISPLAY_SECOND_TENS] = 0;
    v->display[VFD_DISPLAY_SECOND_UNIT] = 0;
    return;
}

void vfd_set_date(VFD *v, Date *d, uint16_t ms){
    #define DATE_DIGITS_SIZE 10
    uint8_t offset = ((ms + 1) / VFD_SCROLL_SPEED) % (VFD_DISPLAY_SIZE+1);          // get offset value
    int8_t date_digits[DATE_DIGITS_SIZE] = {                                        // display data
        -1, -1,                         // skip 2 displays
        2, 0, d->year/10, d->year%10,   // year
        d->month/10, d->month%10,       // month
        d->day/10, d->day%10            // day
    };
    uint8_t disp_order[VFD_DISPLAY_SIZE] = {                                        // display order
        VFD_DISPLAY_HOUR_TENS,
        VFD_DISPLAY_HOUR_UNIT,
        VFD_DISPLAY_MINUTE_TENS,
        VFD_DISPLAY_MINUTE_UNIT,
        VFD_DISPLAY_SECOND_TENS,
        VFD_DISPLAY_SECOND_UNIT
    };

    int8_t i;
    for (i = 0; i < VFD_DISPLAY_SIZE; i++){
        v->display[disp_order[i]] = 0;                                              // by default write nothing to the display
        if (i + offset < DATE_DIGITS_SIZE){                                         // check if index is between bounds
            if (date_digits[i+offset] != -1){                                       // check value of digit
                v->display[disp_order[i]] = vfd_font_map(date_digits[i+offset]);    // write digit to display
            }
        }
    }

    return;
}
