

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

