/*
 *  IO lib
 *  pyroesp
 */

#include "io.h"


void io_init(IO *pin, uint8_t *out, uint8_t *in, uint8_t bit, uint8_t shift){
    pin->bit = bit;
    pin->in = in;
    pin->out = out;
    return;
}

uint8_t io_read_pin(IO *pin){
    return ((*pin->in) >> pin->bit) & 0x01;
}

void io_write_pin(IO *pin, uint8_t v){
    if (v){
        (*pin->out) |= (1 << pin->bit);
    } else {
        (*pin->out) &= ~(1 << pin->bit);
    }
    return;
}

void io_write_port(IO *port, uint8_t value){
    (*port->out) = ((*port->out) & ~port->bit) | ((value << port->shift) & port->bit);
    return;
}

