/*

  bookmarks.c
  
  U8glib Example (root change callback procedure, HOME key)

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

  Preconditions:
    Assign a suitable HOME button

  Idea:
    - Store selected menus in a list with favorite menus
    - Quickly jump back to a menu from your personal favorites

  Notes:
    - This example contains eight random menus and one bookmark menu
    - Menu names are stored in constant strings to reduce memory consumption.
    - The root change callback remembers the last menu and finds the correct name
    - The HOME key will jump to a bookmark menu

*/
#include "u8g.h"
#include "m2.h"
#include "m2utl.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

//=======================================
//=================================================


char bm_m1_name[] = "Menu 1";
M2_EXTERN_ALIGN(el_bm_m1);
char bm_m2_name[] = "Menu 2";
M2_EXTERN_ALIGN(el_bm_m2);
char bm_m3_name[] = "Menu 3";
M2_EXTERN_ALIGN(el_bm_m3);
char bm_m4_name[] = "Menu 4";
M2_EXTERN_ALIGN(el_bm_m4);

char bm_m5_name[] = "Menu 5";
M2_EXTERN_ALIGN(el_bm_m5);
char bm_m6_name[] = "Menu 6";
M2_EXTERN_ALIGN(el_bm_m6);
char bm_m7_name[] = "Menu 7";
M2_EXTERN_ALIGN(el_bm_m7);
char bm_m8_name[] = "Menu 8";
M2_EXTERN_ALIGN(el_bm_m8);

M2_EXTERN_ALIGN(el_bm_home);


#define BM_MAX 6
const char *bm_name[BM_MAX] = { "", "", "", "", "", "" };
m2_rom_void_p  bm_menu[BM_MAX] = { NULL, NULL, NULL, NULL, NULL, NULL };
const char *bm_last_name = "";
m2_rom_void_p bm_last_menu = NULL;

/* variables for the STRLIST element with all bookmarks */
uint8_t bm_list_cnt = BM_MAX;
uint8_t bm_list_first = 0;

/* Is this menu already a bookmark? */
uint8_t bm_is_bookmark(m2_rom_void_p menu) {
  uint8_t i;
  for ( i = 0; i < BM_MAX; i++ ) {
    if ( bm_menu[i] ==  menu )
      return 1;
  }
  return 0;
}

/* Add a new bookmark the bookmark list. Add the new bookmark at the beginning of the list. */
void bm_add_menu(const char *name, m2_rom_void_p menu) {
  uint8_t i;
  for ( i = BM_MAX-1; i > 0; i-- ) {
    bm_name[i] = bm_name[i-1];
    bm_menu[i] = bm_menu[i-1];
  }
  bm_name[0] = name;
  bm_menu[0] = menu;
}

/* root change callback to support the bookmark menu */
void bm_root_change_cb(m2_rom_void_p new_root, m2_rom_void_p old_root, uint8_t change_value)
{
  if ( old_root == &el_bm_home && new_root == &el_bm_home ) {
    /* User has pressed home, while in home menu, try to go back to previous menu */
    if ( bm_last_menu != NULL )
      m2_SetRoot(bm_last_menu);
  }
  if ( old_root == &el_bm_m1 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m1_name;
  }
  if ( old_root == &el_bm_m2 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m2_name;
  }
  if ( old_root == &el_bm_m3 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m3_name;
  }
  if ( old_root == &el_bm_m4 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m4_name;
  }
  if ( old_root == &el_bm_m5 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m5_name;
  }
  if ( old_root == &el_bm_m6 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m6_name;
  }
  if ( old_root == &el_bm_m7 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m7_name;
  }
  if ( old_root == &el_bm_m8 )  {
    bm_last_menu = old_root;
    bm_last_name = bm_m8_name;
  }
}

/*--- Home Menu ---*/

void bm_add_bookmark_button_function(m2_el_fnarg_p fnarg) {
    if ( bm_last_menu != NULL ) {
      /* check if the menu is already bookmarked */
      if ( bm_is_bookmark(bm_last_menu) == 0 ) {
	/* if not, add it to the list */
	bm_add_menu(bm_last_name, bm_last_menu);
      }
      m2_SetRoot(bm_last_menu);
    }
}

void bm_return_to_last_menu_cb(m2_el_fnarg_p fnarg) {
    if ( bm_last_menu != NULL )
      m2_SetRoot(bm_last_menu);
}

const char *bm_list_cb(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT ) {
    if ( bm_menu[idx] != NULL ) {
      m2_SetRoot(bm_menu[idx]);
    }
  }
  return bm_name[idx];
}

M2_LABEL(el_bm_home_label, NULL, "Last Menu:");
M2_BUTTONPTR(el_bm_home_last, "f4", &bm_last_name, bm_return_to_last_menu_cb);
M2_BUTTON(el_bm_add, "f4", "Add as bookmark", bm_add_bookmark_button_function);


M2_STRLIST(el_bm_list_strlist, "l2W52", &bm_list_first, &bm_list_cnt, bm_list_cb);
M2_SPACE(el_bm_list_space, "W1h1");
M2_VSB(el_bm_list_vsb, "l2W2r1", &bm_list_first, &bm_list_cnt);
M2_LIST(list_bm_list) = { &el_bm_list_strlist, &el_bm_list_space, &el_bm_list_vsb };
M2_HLIST(el_bm_list_hlist, NULL, list_bm_list);

M2_LIST(list_bm_home) = { &el_bm_home_label, &el_bm_home_last, &el_bm_add, &el_bm_list_hlist };
M2_VLIST(el_bm_home_vl, NULL, list_bm_home);
M2_ALIGN(el_bm_home, "W64H64", &el_bm_home_vl);


/*--- Menu 1 ---*/
M2_LABEL(el_bm_m1_label, NULL, bm_m1_name);
M2_LABEL(el_bm_m1_home_info, NULL, "Press Home Button");
M2_ROOT(el_bm_m1_b1, "f4", bm_m2_name, &el_bm_m2);
M2_ROOT(el_bm_m1_b2, "f4", bm_m5_name, &el_bm_m5);
M2_LIST(list_bm_m1) = { &el_bm_m1_label, &el_bm_m1_home_info, &el_bm_m1_b1, &el_bm_m1_b2 };
M2_VLIST(el_bm_m1_vl, NULL, list_bm_m1);
M2_ALIGN(el_bm_m1, "W64H64", &el_bm_m1_vl);

/*--- Menu 2 ---*/
M2_LABEL(el_bm_m2_label, NULL, bm_m2_name);
M2_ROOT(el_bm_m2_b1, "f4", bm_m7_name, &el_bm_m7);
M2_ROOT(el_bm_m2_b2, "f4", bm_m3_name, &el_bm_m3);
M2_LIST(list_bm_m2) = { &el_bm_m2_label, &el_bm_m2_b1, &el_bm_m2_b2 };
M2_VLIST(el_bm_m2_vl, NULL, list_bm_m2);
M2_ALIGN(el_bm_m2, "W64H64", &el_bm_m2_vl);

/*--- Menu 3 ---*/
M2_LABEL(el_bm_m3_label, NULL, bm_m3_name);
M2_ROOT(el_bm_m3_b1, "f4", bm_m4_name, &el_bm_m4);
M2_ROOT(el_bm_m3_b2, "f4", bm_m8_name, &el_bm_m8);
M2_LIST(list_bm_m3) = { &el_bm_m3_label, &el_bm_m3_b1, &el_bm_m3_b2 };
M2_VLIST(el_bm_m3_vl, NULL, list_bm_m3);
M2_ALIGN(el_bm_m3, "W64H64", &el_bm_m3_vl);

/*--- Menu 4 ---*/
M2_LABEL(el_bm_m4_label, NULL, bm_m4_name);
M2_ROOT(el_bm_m4_b1, "f4", bm_m5_name, &el_bm_m5);
M2_ROOT(el_bm_m4_b2, "f4", bm_m3_name, &el_bm_m3);
M2_LIST(list_bm_m4) = { &el_bm_m4_label, &el_bm_m4_b1, &el_bm_m4_b2 };
M2_VLIST(el_bm_m4_vl, NULL, list_bm_m4);
M2_ALIGN(el_bm_m4, "W64H64", &el_bm_m4_vl);

/*--- Menu 5 ---*/
M2_LABEL(el_bm_m5_label, NULL, bm_m5_name);
M2_ROOT(el_bm_m5_b1, "f4", bm_m2_name, &el_bm_m2);
M2_ROOT(el_bm_m5_b2, "f4", bm_m6_name, &el_bm_m6);
M2_LIST(list_bm_m5) = { &el_bm_m5_label, &el_bm_m5_b1, &el_bm_m5_b2 };
M2_VLIST(el_bm_m5_vl, NULL, list_bm_m5);
M2_ALIGN(el_bm_m5, "W64H64", &el_bm_m5_vl);

/*--- Menu 6 ---*/
M2_LABEL(el_bm_m6_label, NULL, bm_m6_name);
M2_ROOT(el_bm_m6_b1, "f4", bm_m5_name, &el_bm_m5);
M2_ROOT(el_bm_m6_b2, "f4", bm_m7_name, &el_bm_m7);
M2_LIST(list_bm_m6) = { &el_bm_m6_label, &el_bm_m6_b1, &el_bm_m6_b2 };
M2_VLIST(el_bm_m6_vl, NULL, list_bm_m6);
M2_ALIGN(el_bm_m6, "W64H64", &el_bm_m6_vl);

/*--- Menu 7 ---*/
M2_LABEL(el_bm_m7_label, NULL, bm_m7_name);
M2_ROOT(el_bm_m7_b1, "f4", bm_m1_name, &el_bm_m1);
M2_ROOT(el_bm_m7_b2, "f4", bm_m3_name, &el_bm_m3);
M2_LIST(list_bm_m7) = { &el_bm_m7_label, &el_bm_m7_b1, &el_bm_m7_b2 };
M2_VLIST(el_bm_m7_vl, NULL, list_bm_m7);
M2_ALIGN(el_bm_m7, "W64H64", &el_bm_m7_vl);

/*--- Menu 8 ---*/
M2_LABEL(el_bm_m8_label, NULL, bm_m8_name);
M2_ROOT(el_bm_m8_b1, "f4", bm_m6_name, &el_bm_m6);
M2_ROOT(el_bm_m8_b2, "f4", bm_m2_name, &el_bm_m2);
M2_LIST(list_bm_m8) = { &el_bm_m8_label, &el_bm_m8_b1, &el_bm_m8_b2 };
M2_VLIST(el_bm_m8_vl, NULL, list_bm_m8);
M2_ALIGN(el_bm_m8, "W64H64", &el_bm_m8_vl);



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
// m2 object and constructor
  m2_Init(&el_bm_m1, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bf);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);

  /* 5. Define keys (PN() description, see http://code.google.com/p/u8glib/wiki/avr#U8glib_Init) */
  m2_SetPin(M2_KEY_HOME, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));

  // Set menu for the HOME key
  m2_SetHome(&el_bm_home);
  
  // Register a root change callback procedure
  m2_SetRootChangeCallback(bm_root_change_cb);

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

