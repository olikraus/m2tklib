/*

  U32cb.pde
  
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
  
  SelectPin = 0;
  DownPin = 1;
  UpPin = 2;
  

*/

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"
#include "fonts/Arial14.h"         

uint8_t uiKeySelectPin = 0;
uint8_t uiKeyDownPin = 1;
uint8_t uiKeyUpPin = 2;
//uint8_t uiKeyExitPin = 0;

uint32_t number = 1234;

const char *fn_label(m2_rom_void_p element) {
  static const char label[] = "Num:";
  return label;
}

uint32_t fn_u32(m2_rom_void_p element, uint8_t msg, uint32_t val) {
  if ( msg == M2_U32_MSG_SET_VALUE )
    number = val;
  return number;
}


void fn_ok(m2_el_fnarg_p fnarg) {
  /* do something with the number */
}

M2_LABELFN(el_label, NULL, fn_label);
M2_U32NUMFN(el_num, "a1c4", fn_u32);
M2_BUTTON(el_ok, "", " ok ", fn_ok);
M2_LIST(list) = { &el_label, &el_num, &el_ok };
M2_HLIST(top_el_hlist, NULL, list);
M2tk m2(&top_el_hlist, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_uffs);

void setup() {
  m2.setFont(0, m2_Arial14);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  //m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}
