/*
 *  VFD lib
 *  pyroesp
 */

#ifndef __VFD_H
#define	__VFD_H

#include <stdint.h>
#include "time.h"
#include "date.h"
#include "io.h"

// Font size
#define VFD_FONT_SIZE 36
// 7 seg displays
#define VFD_DISPLAY_SIZE 6
// Scroll speed
#define VFD_SCROLL_SPEED 250

// VFD segment bits
#define VFD_BIT_A 0
#define VFD_BIT_B 1
#define VFD_BIT_C 2
#define VFD_BIT_D 3
#define VFD_BIT_E 4
#define VFD_BIT_F 5
#define VFD_BIT_G 6
#define VFD_BIT_DOT 7

// VFD data position
#define VFD_DISPLAY_SECOND_UNIT 0
#define VFD_DISPLAY_SECOND_TENS 1
#define VFD_DISPLAY_MINUTE_UNIT 2
#define VFD_DISPLAY_MINUTE_TENS 3
#define VFD_DISPLAY_HOUR_UNIT 4
#define VFD_DISPLAY_HOUR_TENS 5

// VFD off value
#define VFD_SEGMENT_OFF 0

// VFD struct
typedef struct{
    IO segment;                         // IO for segment
    IO grid;                            // IO for grid
    uint8_t display[VFD_DISPLAY_SIZE];  // display data array
    uint8_t index;                      // index
}VFD;

// VFD init
void vfd_init(VFD *v);
// VFD set io
void vfd_set_io(IO *port, uint8_t *out, uint8_t bit, uint8_t shift);
// VFD get font from value
uint8_t vfd_font_map(uint8_t value);
// VFD set dot
void vfd_set_dot(uint8_t *display, uint8_t dot);
// VFD write IO
void vfd_write_io(VFD *v, uint8_t segment, uint8_t grid);
// VFD write all - to be executed as fast as possible
void vfd_write_all(VFD *v);
// VFD write time
void vfd_set_time(VFD *v, Time *t);
// VFD write alarm
void vfd_set_alarm(VFD *v, Time *t, uint8_t alarm, uint8_t enabled);
// VFD write day of week
void vfd_set_day_of_week(VFD *v, uint8_t day_of_week);
// VFD write date
void vfd_set_date(VFD *v, Date *d, uint16_t ms);


#endif
