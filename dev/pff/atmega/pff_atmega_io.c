/*

  pff_atmega_io.c
  
  atmega low level procedures for pff

  part of m2tklib (Mini Interative Interface Toolkit Library)
  
  Copyright (C) 2012  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/*
  Delay by the provided number of microseconds.
  Thus, a 16 bit value will allow a delay of 0..65 milliseconds
  Makes use of the _delay_loop_2
  
  _delay_loop_2 will do a delay of n * 4 prozessor cycles.
  with f = F_CPU cycles per second,
  n = f / (10000000 * 4 )
  
  
  with f = 16000000 the result is 4
  with f = 4000000 the result is 1
  
  the millisec loop, gcc requires the following overhead:
  - movev 1
  - subwi 2x2
  - bne i 2
  ==> 7 cycles
  ==> must be devided by 4, rounded up 7/4 = 2
  
  lowest two bits are ignored: 
*/
void pff_delay_us(uint16_t val)
{
  val += 3;
  val >>= 2;
  while( val != 0 )
  {
    _delay_loop_2( (F_CPU / 1000000L ) -2);
    val--;
  }
}

#ifndef MOSI_DDR
#define MOSI_DDR DDRB
#endif
#ifndef MISO_DDR
#define MISO_DDR DDRB
#endif
#ifndef SCK_DDR
#define SCK_DDR DDRB
#endif
#ifndef SS_DDR
#define SS_DDR DDRB
#endif

#ifndef MOSI_BIT
#define MOSI_BIT 3
#endif
#ifndef MISO_BIT
#define MISO_BIT 4
#endif
#ifndef SCK_BIT
#define SCK_BIT 5
#endif
#ifndef SS_BIT
#define SS_BIT 2
#endif

#ifndef SS_PORT
#define SS_PORT DDRB
#endif

void pff_chip_select_high(void)
{
  SS_PORT |= _BV(SS_BIT);
}

void pff_chip_select_low(void)
{
  SS_PORT &= ~_BV(SS_BIT);
}

void pff_spi_init(void)
{
  
  MOSI_DDR |= _BV(MOSI_BIT);
  MISO_DDR &= ~_BV(MISO_BIT);
  SCK_DDR |= _BV(SCK_BIT);
  SS_DDR |= _BV(SS_BIT);
  
  /*
    SPR1 SPR0
        0	0		fclk/4
        0	1		fclk/16
        1	0		fclk/64
        1	1		fclk/128
  */
  SPCR = 0;
  SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(0<<CPOL)|(0<<CPHA);
}


uint8_t pff_spi_out(uint8_t data)
{
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR and return incoming value */
  return  SPDR;
}
