/*
 *  USART lib
 *  pyroesp
 */

#ifndef __USART_H
#define	__USART_H

#include <stdint.h>

// USART struct
typedef struct{
    void (*usart_flush_rx)(void);
    uint8_t (*usart_check_rx_overflow)(void);
    uint8_t (*usart_check_rx_complete)(void);
    uint8_t (*usart_check_tx_complete)(void);
    void (*usart_write)(uint8_t v);
}Usart;

void usart_init(Usart *u);

#endif

