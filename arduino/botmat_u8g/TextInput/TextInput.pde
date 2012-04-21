/*

  TextInput.pde
  
  M2tklib Botmat Example

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

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"

//U8GLIB_DOGM128 u8g(7, 5, 1, 2);                    // SPI Com: SCK = 7, MOSI = 5, CS = 1, A0 = 2
U8GLIB_DOGM128 u8g(1, 2);                    // HW SPI CS = 1, A0 = 2

#define TEXT_LEN 12
char text[] = "abcdefghijkl";

void fn_text_ok(m2_el_fnarg_p fnarg) {
  /* do something with the text */
}

M2_LABEL(el_label, "", "Enter Text:");
M2_TEXT(el_text, "a1", text, TEXT_LEN);
M2_BUTTON(el_ok, "", " ok ", fn_text_ok);
M2_LIST(list) = { &el_label, &el_text, &el_ok };
M2_VLIST(list_element, NULL, list);
M2tk m2(&list_element, m2_es_arduino, m2_eh_6bs, m2_gh_u8g_ffs);

// U8glib draw procedure: Just call the M2tklib draw procedure
void draw(void) {
    m2.draw();
}

// Arduino setup procedure (called only once)
void setup() {
  // activate backlight
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x13);

  // Setup keys (botmat)
  m2.setPin(M2_KEY_SELECT, 18);
  m2.setPin(M2_KEY_PREV, 20);
  m2.setPin(M2_KEY_NEXT, 21);
  m2.setPin(M2_KEY_DATA_UP, 22);
  m2.setPin(M2_KEY_DATA_DOWN, 19);
}

// Arduino loop procedure
void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}


