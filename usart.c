/*
 *  USART lib
 *  pyroesp
 */

#include "usart.h"


void usart_init(Usart *u){
    u->usart_flush_rx = 0;
    u->usart_check_rx_overflow = 0;
    u->usart_check_rx_complete = 0;
    u->usart_check_tx_complete = 0;
    u->usart_write = 0;
    return;
}