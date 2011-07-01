/*

  DataEntry.pde

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

void fn_zero(m2_el_fnarg_p fnarg);
M2_EXTERN_VLIST(el_text_menu);

extern M2tk m2;

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;


/*=============================*/

uint8_t u8num = 0;
uint32_t u32num = 0;

M2_LABEL(el_label1, NULL, "U8:");
M2_U8NUM(el_1, NULL, 0, 255, &u8num);

M2_LABEL(el_label2, NULL, "U32:");
M2_U32NUM(el_2, "c5", &u32num);

M2_LABEL(el_label3, NULL, "");
M2_U32NUM(el_3, "a1c5f0", &u32num);

M2_BUTTON(el_zero, "f4", " zero ", fn_zero);

M2_LIST(de_list) = { 
    &el_label1, &el_1, 
    &el_label2, &el_2,  
    &el_label3, &el_3, 
    &el_zero, 
};
M2_GRIDLIST(el_de_menu, "c2", de_list);

/*=============================*/



M2tk m2(&el_de_menu, m2_es_arduino, m2_eh_4bd, m2_gh_glcd_ffs);


void fn_zero(m2_el_fnarg_p fnarg) {
  u8num = 0;
  u32num = 0;
}

void setup() {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}


