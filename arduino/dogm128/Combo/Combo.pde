/*

  Combo.pde

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

uint8_t select_color = 0;
uint8_t select_priority = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}

const char *fn_idx_to_color(uint8_t idx)
{
  if ( idx == 0 )
    return "red";
  else if (idx == 1 )
    return "green";
  return "blue";
}

const char *fn_idx_to_priority(uint8_t idx)
{
  switch(idx)
  {
    case 0: return "lowest";
    case 1: return "low";
    case 2: return "medium";
    case 3: return "high";
    case 4: return "highest";
  }
  return "";
}


M2_LABEL(el_label1, NULL, "Color:");
M2_COMBO(el_combo1, NULL, &select_color, 3, fn_idx_to_color);

M2_LABEL(el_label2, NULL, "Priority: ");
M2_COMBO(el_combo2, "v1", &select_priority, 5, fn_idx_to_priority);

M2_BUTTON(el_cancel, NULL, "cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " ok ", fn_ok);

M2_LIST(list) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(list_element, "c2",list);
M2tk m2(&list_element, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

void setup() {
  m2.setFont(0, font_5x7);
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


