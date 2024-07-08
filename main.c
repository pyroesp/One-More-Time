/*
 *  VFD Clock
 *      + ATmega4808
 *      + Display IV-22
 *          - Segments: PD0~PD7
 *          - Grid:     PA2~PA7
 *      + Buttons
 *          - alarm:    PC0
 *          - hour:     PC1
 *          - minute:   PC2
 *          - snooze:   PC3
 *      + LED
 *          - output:   PF2
 *      + MP3 player module (UART)
 *          - TX/RX:    PA0~PA1
 *      + ESP32 (UART) (OPTIONAL)
 *          - TX/RX:    PF0~PF1
 *          - Sync to NTP
 *          - Setup alarms from ESP32 webserver (?)
 * 
 *  pyroesp
 */

#include <xc.h>
#include "clock.h"


// #TODO: Timer interrupts

int main(void) {
    Clock clock;
    clock_init(&clock);
    
    // assign buttons
    button_set_io(&clock.buttons[BUTTON_ALARM].btn, (uint8_t*)&PORTC_IN, 0);    // PC0
    button_set_io(&clock.buttons[BUTTON_HOUR].btn, (uint8_t*)&PORTC_IN, 1);     // PC1
    button_set_io(&clock.buttons[BUTTON_MINUTE].btn, (uint8_t*)&PORTC_IN, 2);   // PC2
    button_set_io(&clock.buttons[BUTTON_SNOOZE].btn, (uint8_t*)&PORTC_IN, 3);   // PC3
    button_copy_state(clock.buttons, clock.prev_buttons, BUTTON_SIZE);
    
    // assign VFD io
    vfd_set_io(&clock.vfd.grid, (uint8_t*)&PORTA_OUT, 0xFC, 2);     // PA2~PA7
    vfd_set_io(&clock.vfd.segment, (uint8_t*)&PORTD_OUT, 0xFF, 0);  // PD0~PD7
    
    // assign LED
    io_init(&clock.led, (uint8_t*)&PORTF_OUT, 0, 2, 0); // PF2
    
    // main loop
    for(;;){
        //  if timer 1s
        //      time_update(&clock.time);
        
        //  if timer_25ms
        //      clock_update(&clock, 25);
        //  else always update vfd
        //      vfd_write_all(&clock.vfd);
        
        // turn on LED if alarm enabled
        io_write_pin(&clock.led, clock.alarms[clock.date.day_of_week].enabled);
    }
    return 0;
}
