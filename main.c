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
 *      + MP3 player module (USART0)
 *          - TX/RX:    PA0~PA1
 *      + ESP32 (USART2) (OPTIONAL)
 *          - TX/RX:    PF0~PF1
 *          - Sync to NTP
 *          - Setup alarms from ESP32 webserver (?)
 * 
 *  pyroesp
 */


#include <xc.h>
#include "clock.h"


// #TODO: Timer interrupts


void usart0_flush_rx(void);
uint8_t usart0_check_rx_overflow(void);
uint8_t usart0_check_rx_complete(void);
uint8_t usart0_check_tx_complete(void);
void usart0_write(uint8_t v);
void usart2_flush_rx(void);
uint8_t usart2_check_rx_overflow(void);
uint8_t usart2_check_rx_complete(void);
uint8_t usart2_check_tx_complete(void);
void usart2_write(uint8_t v);


int main(void) {
    // MCU init
    // IO
    PORTD.DIRSET = 0xFF;    // segment outputs PD0~PD7
    PORTA.DIRSET = 0xFC;    // grid outputs PA2~PA7
    PORTA.DIRSET = 0x01;    // USART TX PA0
    PORTA.DIRCLR = 0x02;    // USART RX PA1
    PORTF.DIRSET = 0x04;    // led output PF2
    PORTF.DIRSET = 0x01;    // USART TX PF0
    PORTF.DIRCLR = 0x02;    // USART RX PF1
    PORTC.DIRCLR = 0x0F;    // buttons input
    
    PORTC.PIN0CTRL |= PORT_PULLUPEN_bm;  // button pull-up
    PORTC.PIN1CTRL |= PORT_PULLUPEN_bm;  // button pull-up
    PORTC.PIN2CTRL |= PORT_PULLUPEN_bm;  // button pull-up
    PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;  // button pull-up
    
    //USART0 - PA0~PA1 (normal mode + RX + TX)
    USART0.CTRLA = 0x00;                // no interrupts
    USART0.CTRLB = 0xC0;                // enable RX & TX
    USART0.CTRLC = 0x03;                // async mode, no parity, 1 stop bit, 8 data bits
    
    //USART2 - PF0~PF1 (normal mode + RX + TX)
    USART2.CTRLA = 0x00;                // no interrupts
    USART2.CTRLB = 0xC0;                // enable RX & TX
    USART2.CTRLC = 0x03;                // async mode, no parity, 1 stop bit, 8 data bits
    
    
    
    // Clock init
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
    
    // assign USART0 to MP3 com
    clock.mp3.com.usart_flush_rx = &usart0_flush_rx;
    clock.mp3.com.usart_check_rx_overflow = &usart0_check_rx_overflow;
    clock.mp3.com.usart_check_rx_complete = &usart0_check_rx_complete;
    clock.mp3.com.usart_check_tx_complete = &usart0_check_tx_complete;
    clock.mp3.com.usart_write = &usart0_write;
    
    
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


void usart0_flush_rx(void){
    uint8_t t;
    while(USART0.STATUS & USART_RXSIF_bm){
        t = USART0.RXDATAH;
        t = USART0.RXDATAL;
    }
    return;
}

uint8_t usart0_check_rx_overflow(void){
    return (USART0.RXDATAH & USART_BUFOVF_bm) >> USART_BUFOVF_bp;
}

uint8_t usart0_check_rx_complete(void){
    return (USART0.STATUS & USART_RXCIF_bm) >> USART_RXCIF_bp;
}

uint8_t usart0_check_tx_complete(void){
    return (USART0.STATUS & USART_TXCIF_bm) >> USART_TXCIF_bp;
}

void usart0_write(uint8_t v){
    USART0.TXDATAL = v;
    return;
}


void usart2_flush_rx(void){
    uint8_t t;
    while(USART2.STATUS & USART_RXSIF_bm){
        t = USART2.RXDATAH;
        t = USART2.RXDATAL;
    }
    return;
}

uint8_t usart2_check_rx_overflow(void){
    return (USART2.RXDATAH & USART_BUFOVF_bm) >> USART_BUFOVF_bp;
}

uint8_t usart2_check_rx_complete(void){
    return (USART2.STATUS & USART_RXCIF_bm) >> USART_RXCIF_bp;
}

uint8_t usart2_check_tx_complete(void){
    return (USART2.STATUS & USART_TXCIF_bm) >> USART_TXCIF_bp;
}

void usart2_write(uint8_t v){
    USART2.TXDATAL = v;
    return;
}