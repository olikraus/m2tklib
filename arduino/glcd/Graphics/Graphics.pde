/*

  Graphic.pde

  How to mix Graphics with M2tklib
  
  GLCD Example

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
  
  Two methods:
    - mix/combine menu and graphics
    - switch between menu and graphcs
    
   Graphic procedure should be build in parallel
   Graphic procedure should work depending on the top level element
   
*/

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;

uint8_t y = 0;                   // position of the low level graphics

extern M2tk m2;
M2_EXTERN_ALIGN(el_top);

/* draw an animated 10x10 rectange at x/y */
void rectangle(uint8_t x, uint8_t y) {
  static uint8_t is_bigger = 0;
  static uint32_t next_state_change = 0;
  static uint32_t next_redraw = 0;
  static uint8_t size = 1;
  if ( next_redraw < millis() ) {
    next_redraw = millis();
    next_redraw += 30;
    GLCD.FillRect(x,y,10,10, WHITE);
    GLCD.FillRect(x,y,size,size, BLACK);
  }
  if ( next_state_change < millis() ) {
    next_state_change = millis();
    next_state_change += 300;
    if ( is_bigger == 0 ) {
      size -= 1;
      if ( size <= 1 )
        is_bigger = 1;
    }
    else {
      size += 1;
      if ( size >= 10 )
        is_bigger = 0;
    }
  }
}


void fn_inc(m2_el_fnarg_p fnarg) {
  if ( y < 63-10 )
    y += 1;
}

void fn_dec(m2_el_fnarg_p fnarg) {
  if ( y > 0 )
    y -= 1;
}

M2_BUTTON(el_plus, "x30y41", "-1", fn_dec);
M2_BUTTON(el_minus, "x30y21", "+1", fn_inc);
M2_ROOT(el_leave_combine, "x30y1", "Back", &el_top);
M2_LIST(el_btn_list) = { &el_plus, &el_minus, &el_leave_combine};
M2_XYLIST(el_combine, NULL, el_btn_list);

M2_LABEL(el_goto_title, NULL, "Graphics and M2tk");
M2_ROOT(el_goto_combine, NULL, "Combine", &el_combine);
M2_ROOT(el_goto_switch, NULL, "Switch", &m2_null_element);             // selecting this, will remove all menues
M2_LIST(list_menu) = {&el_goto_title, &el_goto_combine, &el_goto_switch};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W64H64", &el_menu_vlist);
M2tk m2(&el_top, m2_es_arduino, m2_eh_4bs, m2_gh_glcd_ffs);

void setup(void) {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}


void graphics(void) {
  // show the graphics depending on the current toplevel element
  
  if ( m2.getRoot() == &el_combine ) {
      // combine is active, do add the rectangle
      // menu is on the right, put the rectangle to the left
      rectangle(0,y);
  }

  if ( m2.getRoot() == &m2_null_element ) {
      // all menus are gone, show the rectangle
      rectangle(10,10);
      // now check for any keys and assign a suitable menu again
      if ( m2.getKey() != M2_KEY_NONE )
        m2.setRoot(&el_top);
  }
}

void menu(void) {
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}

void loop(void) {
  graphics();
  menu();
}


