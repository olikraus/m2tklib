

#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>

const char *u32toa(uint32_t v, uint8_t d);

extern uint32_t stackmon_upper_limit;
extern uint32_t stackmon_start_adr;


void stackmon_Init(void) __attribute__ ((noinline));
uint32_t stackmon_GetUsage(void) __attribute__ ((noinline));


#endif


