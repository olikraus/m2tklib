/*

  pff_arduino_io.cpp
  
  arduino low level procedures for pff
  THIS FILE DOES NOT WORK WITH ARDUINO DUE

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

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#endif

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
  

/*=========================================================================*/
/* setup hardware SPI Arduino pin numbers */
  
#define PFF_MOSI_PIN MOSI
#define PFF_MISO_PIN MISO
#define PFF_SCK_PIN SCK
//#define PFF_CS_PIN SS
#define PFF_CS_PIN 23


/*=========================================================================*/

uint8_t pff_arduino_chip_select_pin = PFF_CS_PIN;

/*=========================================================================*/




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
extern "C" void pff_delay_us(uint16_t val)
{
#if defined(__AVR__)
  val += 3;
  val >>= 2;
  while( val != 0 )
  {
    _delay_loop_2( (F_CPU / 1000000L ) -2);
    val--;
  }
#endif
}

extern "C" void pff_chip_select_high(void)
{
  digitalWrite(pff_arduino_chip_select_pin, HIGH);
}

extern "C" void pff_chip_select_low(void)
{
  digitalWrite(pff_arduino_chip_select_pin, LOW);
}

extern "C" void pff_spi_init(void)
{
#if defined(__AVR__)	
  pinMode(PFF_MOSI_PIN, OUTPUT);
  pinMode(PFF_MISO_PIN, INPUT);
  pinMode(PFF_SCK_PIN, OUTPUT);
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  pinMode(pff_arduino_chip_select_pin, OUTPUT);
  
  
  /*
    SPR1 SPR0
        0	0		fclk/4
        0	1		fclk/16
        1	0		fclk/64
        1	1		fclk/128
  */
  SPCR = 0;
  SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(0<<CPOL)|(0<<CPHA);
#endif
}


extern "C" uint8_t pff_spi_out(uint8_t data)
{
#if defined(__AVR__)	
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR and return incoming value */
  return  SPDR;
#endif
}

