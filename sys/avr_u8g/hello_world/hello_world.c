/*

  hello_world.c 
  
  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
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

#include "u8g.h"
#include "m2.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

/*=========================================================================*/
/* menu definitions */

M2_LABEL(hello_world_label, NULL, "Hello World");

/*=========================================================================*/
/* controller, u8g and m2 setup */

/* 
  Software SPI:
  uint8_t u8g_InitSPI(u8g_t *u8g, u8g_dev_t *dev, uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset); 

  Hardware SPI:
  uint8_t u8g_InitHWSPI(u8g_t *u8g, u8g_dev_t *dev, uint8_t cs, uint8_t a0, uint8_t reset);

  Parallel Interface:
  uint8_t u8g_Init8Bit(u8g_t *u8g, u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, 
    uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw, uint8_t reset);

  Visit 
    http://code.google.com/p/u8glib/wiki/device 
  for a list of valid devices (second argument of the constructor).

  The following examples will use the dogm132 device: u8g_dev_st7565_dogm132_sw_spi

  Note: The device must match the setup: For example, do not use a sw_spi device with u8g_InitHWSPI().
*/

u8g_t u8g;

void setup(void)
{  
  /*
    Test Envionment, ATMEGA with the following settings:
    CS: PORTB, Bit 2
    A0: PORTB, Bit 1
    SCK: PORTB, Bit 5
    MOSI: PORTB, Bit 3
  */
  /* 1. Setup and create u8g device */
  u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE);
  /* u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE); */

  /* 2. Setup m2 */
  m2_Init(&hello_world_label, m2_es_avr_u8g, m2_eh_6bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_6x10);
}

void sys_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */


/* draw procedure of the u8g picture loop */
void draw(void)
{
  /* call the m2 draw procedure */
  m2_Draw();
}

/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void)
{
  /* setup controller */
  sys_init();
	
  /* setup u8g and m2 libraries */
  setup();

  /* application main loop */
  for(;;)
  {  
    m2_CheckKey();
    if ( m2_HandleKey() ) 
    {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do
      {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}

