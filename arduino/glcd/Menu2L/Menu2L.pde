/*

  2LMenu.pde

  GLCD Example

  m2tklib = Mini Interative Interface Toolkit Library
  
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

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;


//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_expandable_menu);

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
// Option e15 = first column has a width of 15 pixel
// Option W43 = second column has a width of 43/64 of the display width

M2_2LMENU(el_2lmenu,"l4e15W43",&m2_2lmenu_first,&m2_2lmenu_cnt, m2_2lmenu_data,'+','-','\0');
M2_SPACE(el_space, "W1h1");
M2_VSB(el_vsb, "l4W2r1", &m2_2lmenu_first, &m2_2lmenu_cnt);
M2_LIST(list_2lmenu) = { &el_2lmenu, &el_space, &el_vsb };
M2_HLIST(el_hlist, NULL, list_2lmenu);
M2_ALIGN(top_el_expandable_menu, "-1|1W64H64", &el_hlist);

// m2 object and constructor
M2tk m2(&top_el_expandable_menu, m2_es_arduino, m2_eh_4bs, m2_gh_glcd_ffs);

void setup() {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}


