/*

  rot_enc.c

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
#include "m2utl.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_menu);

//=================================================
// Simple dialog: Input two values n1 and n2

uint8_t n1 = 0;
uint8_t n2 = 0;

M2_LABEL(el_l1, NULL, "value 1:");
M2_U8NUM(el_u1, "c2", 0, 99, &n1);
M2_LABEL(el_l2, NULL, "value 2:");
M2_U8NUM(el_u2, "c2", 0, 99, &n2);

M2_LIST(list) = { &el_l1, &el_u1, &el_l2, &el_u2 };
M2_GRIDLIST(el_gridlist, "c2", list);
M2_ALIGN(top_menu, "-1|1W64H64", &el_gridlist);


/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;

/*=========================================================================*/
/* controller, u8g and m2 setup */

void setup(void)
{  
  /*
    Test Envionment, ATMEGA with the following settings:
    CS: PORTB, Bit 2
    A0: PORTB, Bit 1
    SCK: PORTB, Bit 5
    MOSI: PORTB, Bit 3
  */
  /* 1. Setup and create u8g device: http://code.google.com/p/u8glib/wiki/avr#U8glib_Init */
  /* u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); */
  u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);

  /* 2. Setup m2 */
  //m2_Init(&top_menu, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);
  m2_Init(&top_menu, m2_es_avr_rotary_encoder_u8g, m2_eh_4bd, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);
	
  /* 5. Define keys (PN() description, see http://code.google.com/p/u8glib/wiki/avr#U8glib_Init) */
  /* normal buttons */
  m2_SetPin(M2_KEY_EXIT, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));
  /* incremental rotary encoder */
  m2_SetPin(M2_KEY_ROT_ENC_A, PN(2, 2));
  m2_SetPin(M2_KEY_ROT_ENC_B, PN(2, 3));

}

/*=========================================================================*/
/* system setup */

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

