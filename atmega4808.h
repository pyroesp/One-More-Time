/*
 *  atmega4808 lib
 *  pyroesp
 */

#ifndef __ATMEGA4808_H
#define	__ATMEGA4808_H

#include <xc.h>
#include <stdint.h>


// init atmega4808 functions
void atmega4808_port_init(void);
void atmega4808_usart0_init(void);
void atmega4808_usart2_init(void);


// usart specific functions
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


#endif

