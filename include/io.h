/*
 *  IO lib
 *  pyroesp
 */

#ifndef __IO_H
#define	__IO_H

#include <stdint.h>

// IO struct
typedef struct{
   uint8_t *in;     // pointer to input register
   uint8_t *out;    // pointer to output register
   uint8_t bit;     // pin, or mask if using io_write_port
   uint8_t shift;   // shift
}IO;

// IO init
void io_init(IO *pin, uint8_t *out, uint8_t *in, uint8_t bit, uint8_t shift);
// IO read pin
uint8_t io_read_pin(IO *pin);
// IO write pin
void io_write_pin(IO *pin, uint8_t v);
// IO write port
void io_write_port(IO *port, uint8_t value);

#endif

