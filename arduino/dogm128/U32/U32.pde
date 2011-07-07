/*

  U32.pde
  
  DOGM128-Lib Example

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

  Pins 5, 6	Timer 0
  Pins 9, 10	Timer 1
  Pins 11, 3	Timer 3
  
  DOGS102 Shield: 

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

uint32_t number = 1234;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* do something with the number */
}

M2_LABEL(el_label, NULL, "Num: ");
M2_U32NUM(el_num, "a1c4", &number);
M2_BUTTON(el_ok, "", " ok ", fn_ok);
M2_LIST(list) = { &el_label, &el_num, &el_ok };
M2_HLIST(top_el_hlist, NULL, list);
M2tk m2(&top_el_hlist, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

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


