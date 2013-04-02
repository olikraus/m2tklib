/*

  menu_x2l.c 

  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
  Copyright (C) 2013  olikraus@gmail.com

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

#include <stdlib.h>
#include "u8g.h"
#include "m2.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

/*=========================================================================*/
/* Forward declaration of the toplevel element */
M2_EXTERN_ALIGN(top_el_x2l_menu);


/*=========================================================================*/
/* Define three user menus, just for demonstration. */

M2_ROOT(el_mnu1_sel, "t1", "Menu 1 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu1_sel, "-1|1W64H64", &el_mnu1_sel);

M2_ROOT(el_mnu2_sel, "t1", "Menu 2 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu2_sel, "-1|1W64H64", &el_mnu2_sel);

M2_ROOT(el_mnu3_sel, "t1", "Menu 3 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu3_sel, "-1|1W64H64", &el_mnu3_sel);


/*=========================================================================*/
uint8_t value = 0;
char buf[20];

/* define callback procedure, which returns a menu entry with a value */
const char *xmenu_value(uint8_t idx, uint8_t msg)
{  
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    strcpy(buf, " Value: ");
    itoa((int)value, buf+strlen(buf), 10);
    return buf;
  }
  return "";
}

/* define callback procedures which increment and decrement a value */
const char *xmenu_inc(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      value++;
  }
  return "";
}

const char *xmenu_dec(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      value--;
  }
  return "";
}

/*=========================================================================*/
/* this is the overall menu structure for the X2L Menu */

m2_xmenu_entry xmenu_data[] = 
{
  { "Menu 1", NULL, NULL },		/* expandable main menu entry */
  { ".", NULL, xmenu_value },		/* The label of this menu line is returned by the callback procedure */
  { ". Inc", NULL, xmenu_inc },		/* This callback increments the value */
  { ". Dec", NULL, xmenu_dec },		/* This callback decrements the value */
  { "Menu 2", NULL, NULL },
  { ". Sub 2-1", &top_el_mnu1_sel, NULL },
  { ". Sub 2-2", &top_el_mnu2_sel, NULL},
  { ". Sub 2-3", &top_el_mnu3_sel, NULL },
  { NULL, NULL, NULL },
};

/*=========================================================================*/
/* This is the main menu dialog box */

/* The first visible line and the total number of visible lines. */
/* Both values are written by M2_X2LMENU and read by M2_VSB */
uint8_t el_x2l_first = 0;
uint8_t el_x2l_cnt = 3;

/* M2_X2LMENU definition */
/* Option l4 = four visible lines */
/* Option e15 = first column has a width of 15 pixel */
/* Option W43 = second column has a width of 43/64 of the display width */
/* Option F3 = select font 3 for the extra column (icons) */

M2_X2LMENU(el_x2l_strlist, "l4e15W43F3", &el_x2l_first, &el_x2l_cnt, xmenu_data, 65,102,'\0');
M2_SPACE(el_x2l_space, "W1h1");
M2_VSB(el_x2l_vsb, "l4W2r1", &el_x2l_first, &el_x2l_cnt);
M2_LIST(list_x2l) = { &el_x2l_strlist, &el_x2l_space, &el_x2l_vsb };
M2_HLIST(el_x2l_hlist, NULL, list_x2l);
M2_ALIGN(top_el_x2l_menu, "-1|1W64H64", &el_x2l_hlist);



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
  /* 1. Setup and create u8g device */
  /* u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); */
  u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);

  /* 2. Setup m2 */
  m2_Init(&top_el_x2l_menu, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);
  m2_SetFont(3, (const void *)u8g_font_m2icon_5);
	
  /* 5. Define keys */
  m2_SetPin(M2_KEY_EXIT, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));
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

