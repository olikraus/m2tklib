/*

  ChangeMenu.pde

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

  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include "Dogm.h"
#include "M2tk.h"
#include "m2ghdogm.h"

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);

M2_EXTERN_VLIST(el_list1);
M2_EXTERN_VLIST(el_list2);

M2tk m2(&el_list1, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

void fn_ok1(m2_el_fnarg_p fnarg) {
  m2.setRoot(&el_list2);
}

M2_LABEL(el_label1, NULL, "First Menu");
M2_BUTTON(el_ok1, NULL, " Goto 2 ", fn_ok1);
M2_LIST(list1) = { &el_label1, &el_ok1 };
M2_VLIST(el_list1, NULL, list1);

M2_LABEL(el_label2, NULL, "Second Menu");
M2_ROOT(el_ok2, NULL, " Goto 1 ", &el_list1);
M2_LIST(list2) = { &el_label2, &el_ok2 };
M2_VLIST(el_list2, NULL, list2);

void setup() {
  m2.setFont(0, font_6x12);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
}


