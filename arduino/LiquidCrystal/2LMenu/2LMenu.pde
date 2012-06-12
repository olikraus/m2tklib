/*

  2LMenu.pde
  
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
M2_EXTERN_HLIST(top_el_expandable_menu);

// Left entry: Menu name. Submenus must have a '.' at the beginning
// Right entry: Reference to the target dialog box (In this example all menus call the toplevel element again
m2_menu_entry m2_2lmenu_data[] = 
{
  { "Menu 1", NULL },
  { ". Sub 1-1", &top_el_expandable_menu },
  { ". Sub 1-2", &top_el_expandable_menu },
  { "Menu 2", &top_el_expandable_menu },
  { "Menu 3", NULL },
  { ". Sub 3-1", &top_el_expandable_menu },
  { ". Sub 3-2", &top_el_expandable_menu },
  { "Menu 4", &top_el_expandable_menu },
  { "Menu 5", NULL },
  { ". Sub 5-1", &top_el_expandable_menu },
  { ". Sub 5-2", &top_el_expandable_menu },
  { ". Sub 5-3", &top_el_expandable_menu },
  { NULL, NULL },
};

// The first visible line and the total number of visible lines.
// Both values are written by M2_2LMENU and read by M2_VSB
uint8_t m2_2lmenu_first;
uint8_t m2_2lmenu_cnt;

// M2_2LMENU definition
// Option l4 = four visible lines
// Option e1 = first column has a width of 1 char
// Option w12 = second column has a width of 12 chars

M2_2LMENU(el_2lmenu,"l4e1w12",&m2_2lmenu_first,&m2_2lmenu_cnt, m2_2lmenu_data,'+','-','\0');
M2_VSB(el_vsb, "l4w1r1", &m2_2lmenu_first, &m2_2lmenu_cnt);
M2_LIST(list_2lmenu) = { &el_2lmenu, &el_vsb };
M2_HLIST(top_el_expandable_menu, NULL, list_2lmenu);

// m2 object and constructor
M2tk m2(&top_el_expandable_menu, m2_es_arduino, m2_eh_4bs, m2_gh_lc);

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
