/*

  DataEntry.pde
  
  DOGM128 Library Example

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

void fn_zero(m2_el_fnarg_p fnarg);
extern M2tk m2;

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
uint8_t uiKeyExitPin = 2;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 8; // ????
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);

uint8_t u8num = 0;
uint32_t u32num = 0;

M2_LABEL(el_label1, NULL, "U8:");
M2_U8NUM(el_1, NULL, 0, 255, &u8num);
M2_LABEL(el_label2, NULL, "U32:");
M2_U32NUM(el_2, "c5.2", &u32num);
M2_LABEL(el_label3, NULL, "");
M2_U32NUM(el_3, "a1c5.2", &u32num);
M2_BUTTON(el_zero, "f4", " zero ", fn_zero);
M2_LIST(de_list) = { 
    &el_label1, &el_1, 
    &el_label2, &el_2,  
    &el_label3, &el_3, 
    &el_zero, 
};
M2_GRIDLIST(el_de_menu, "c2", de_list);
M2tk m2(&el_de_menu, m2_es_arduino, m2_eh_4bd, m2_gh_dogm_ffs);

void fn_zero(m2_el_fnarg_p fnarg) {
  u8num = 0;
  u32num = 0;
}

void setup() {
  m2.setFont(0, font_5x7);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
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


