/*
  m2utlu8d
  
  convert u8 to d
  
*/

#include "m2utl.h"

static const unsigned char m2_utl_u8d_tab[3]  = { 100, 10, 1 } ;
const char *m2_utl_u8dp(char * dest, uint8_t v)
{
  uint8_t pos;
  uint8_t d;
  uint8_t c;
  for( pos = 0; pos < 3; pos++ )
  {
      d = '0';
      c = *(m2_utl_u8d_tab+pos);
      while( v >= c )
      {
	v -= c;
	d++;
      }
      dest[pos] = d;
  }  
  dest[3] = '\0';
  return dest;
}

/* v = value, d = number of digits */
const char *m2_utl_u8d(uint8_t v, uint8_t d)
{
  d = 3-d;
  return m2_utl_u8dp(m2_utl_string_conversion_data, v) + d;
}



