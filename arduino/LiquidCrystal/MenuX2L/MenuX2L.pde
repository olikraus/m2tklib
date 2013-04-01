/*

  MenuX2L.pde
  
  LiquidCrystal 16x4 example

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

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

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

uint8_t uiKeySelectPin = 10;
uint8_t uiKeyNextPin = 9;


//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_HLIST(top_el_x2l_menu);


//=================================================
// Define three user menus, just for demonstration. 

M2_ROOT(el_mnu1_sel, "t1", "Menu1 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu1_sel, "-1|1W64H64", &el_mnu1_sel);

M2_ROOT(el_mnu2_sel, "t1", "Menu2 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu2_sel, "-1|1W64H64", &el_mnu2_sel);

M2_ROOT(el_mnu3_sel, "t1", "Menu3 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu3_sel, "-1|1W64H64", &el_mnu3_sel);


//=================================================
uint8_t value = 0;
char buf[20];

// define callback procedure, which returns a menu entry with a value
const char *xmenu_value(uint8_t idx, uint8_t msg)
{  
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    strcpy(buf, " Value: ");
    itoa((int)value, buf+strlen(buf), 10);
    return buf;
  }
  return "";
}

// define callback procedures which increment and decrement a value
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

//=================================================
// this is the overall menu structure for the X2L Menu

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

//=================================================
// This is the main menu dialog box

// The first visible line and the total number of visible lines.
// Both values are written by M2_X2LMENU and read by M2_VSB
uint8_t el_x2l_first = 0;
uint8_t el_x2l_cnt = 3;

// M2_X2LMENU definition
// Option l4 = four visible lines
// Option e1 = first column has a width of 1 char
// Option w12 = second column has a width of 12 chars
M2_X2LMENU(el_x2l_strlist, "l4e1w12", &el_x2l_first, &el_x2l_cnt, xmenu_data, '+','-','\0');
M2_VSB(el_x2l_vsb, "l4W2r1", &el_x2l_first, &el_x2l_cnt);
M2_LIST(list_x2l) = { &el_x2l_strlist, &el_x2l_vsb };
M2_HLIST(top_el_x2l_menu, NULL, list_x2l);

//=================================================
// m2 object and constructor
M2tk m2(&top_el_x2l_menu, m2_es_arduino, m2_eh_4bs, m2_gh_lc);

//================================================================
// Arduino setup and loop
void setup() {
  m2_SetLiquidCrystal(&lcd, 16, 4);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyNextPin);
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}
