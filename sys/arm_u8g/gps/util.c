

#include "util.h"

const char *u32toap(char * dest, uint32_t v)
{
  uint8_t pos;
  uint8_t d;
  uint32_t c;
  c = 1000000000;
  for( pos = 0; pos < 10; pos++ )
  {
      d = '0';
      while( v >= c )
      {
	v -= c;
	d++;
      }
      dest[pos] = d;
      c /= 10UL;
  }  
  dest[10] = '\0';
  return dest;
}

/* v = value, d = number of digits */
const char *u32toa(uint32_t v, uint8_t d)
{
  static char buf[11];
  d = 10-d;
  return u32toap(buf, v) + d;
}



/*
  Taken from core_cm0.c

  Return the Main Stack Pointer

*/
uint32_t stackmon_get_MSP(void) __attribute__( ( naked ) );
uint32_t stackmon_get_MSP(void)
{
  uint32_t result=0;

  __asm volatile ("MRS %0, msp\n\t" 
                  "MOV r0, %0 \n\t"
                  "BX  lr     \n\t"  : "=r" (result) );
  return(result);
}


/* LPC1114 specific */

uint32_t stackmon_upper_limit;
uint32_t stackmon_start_adr;

#define STACKMON_MAGIC 0x0f112223

void stackmon_Init(void)
{
  uint32_t *stackmon_ptr;
  
  stackmon_upper_limit = stackmon_get_MSP();
  stackmon_upper_limit &= ~3;
  stackmon_upper_limit -= 4;
  
  stackmon_start_adr = 0x010001000 - __STACK_SIZE;
  
  stackmon_ptr = (uint32_t *)stackmon_start_adr;
  for(;;)
  {
    if ( (uint32_t)stackmon_ptr >= stackmon_upper_limit )
      break;
    *stackmon_ptr++ = STACKMON_MAGIC;
  }
}

uint32_t stackmon_GetUsage(void)
{
  uint32_t *stackmon_ptr;
  stackmon_ptr = (uint32_t *)stackmon_start_adr;
  for(;;)
  {
    if ( (uint32_t)stackmon_ptr >= stackmon_upper_limit )
      break;
    if ( *stackmon_ptr != STACKMON_MAGIC )
      break;
    stackmon_ptr++;
  }
  
  return 0x010001000 -  (uint32_t)stackmon_ptr;
  
}