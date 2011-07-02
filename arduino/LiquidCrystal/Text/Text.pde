/*

  Text.pde
  
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

#define TEXT_LEN 12
char text[] = "abcdefghijkl";

void fn_text_ok(m2_el_fnarg_p fnarg) {
  /* do something with the text */
}

M2_LABEL(el_label, "", "Enter Text:");
M2_TEXT(el_text, NULL, text, TEXT_LEN);
M2_BUTTON(el_ok, "", " ok ", fn_text_ok);
M2_LIST(list) = { &el_label, &el_text, &el_ok };
M2_VLIST(list_element, NULL, list);
M2tk m2(&list_element, m2_es_arduino, m2_eh_2bs, m2_gh_lc);

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
