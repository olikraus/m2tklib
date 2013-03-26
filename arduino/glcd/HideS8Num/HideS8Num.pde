/*

  HideS8Num.pde

  Demonstration of M2_HIDE and M2_S8NUM
  
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

*/

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;

//================================================================
// m2tklib forward declarations

extern M2tk m2;
M2_EXTERN_ALIGN(el_top);

//================================================================
// M2_HIDE and M2_S8NUM

uint8_t hide_on = 0;
int8_t hide_s8num = 0;


M2_LABEL(el_hide_label, NULL, "Hide: ");
M2_TOGGLE(el_hide_toggle, "", &hide_on);

M2_LABEL(el_hide_num_label, NULL, "Num: ");
M2_HIDE(h_el_hide_num_label, NULL, &hide_on, &el_hide_num_label);
M2_S8NUM(el_hide_num_s8, "+1c2", -19, 19, &hide_s8num);
M2_HIDE(h_el_hide_num_s8, NULL, &hide_on, &el_hide_num_s8);

M2_LIST(hide_example_list) = { 
    &el_hide_label, &el_hide_toggle, 
    &h_el_hide_num_label, &h_el_hide_num_s8
};
M2_GRIDLIST(el_hide_example, "c2", hide_example_list);
M2_ALIGN(el_top, "-1|1W64H64", &el_hide_example);

M2tk m2(&el_top, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_ffs);

//================================================================
// Arduino setup and loop

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
