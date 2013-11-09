

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void UART_Init(uint8_t is_48_mhz, void (*rx_handler)(uint8_t data));


#endif



