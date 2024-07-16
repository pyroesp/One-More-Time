/*
 *  atmega4808 lib
 *  pyroesp
 */

#include "atmega4808.h"


void atmega4808_port_init(void){
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
}

void atmega4808_usart0_init(void){
    //USART0 - PA0~PA1 (normal mode + RX + TX)
    USART0.CTRLA = 0x00;                // no interrupts
    USART0.CTRLB = 0xC0;                // enable RX & TX
    USART0.CTRLC = 0x03;                // async mode, no parity, 1 stop bit, 8 data bits
}

void atmega4808_usart2_init(void){
    //USART2 - PF0~PF1 (normal mode + RX + TX)
    USART2.CTRLA = 0x00;                // no interrupts
    USART2.CTRLB = 0xC0;                // enable RX & TX
    USART2.CTRLC = 0x03;                // async mode, no parity, 1 stop bit, 8 data bits
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