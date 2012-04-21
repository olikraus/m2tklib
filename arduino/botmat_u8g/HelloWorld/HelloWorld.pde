/*

  HelloWorld.pde
  
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


M2_LABEL(hello_world_label, NULL, "Hello World");
M2tk m2(&hello_world_label, NULL, NULL, m2_gh_u8g_bfs);

void draw(void) {
    m2.draw();
}

void setup() {
  // activate backlight
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  /* connect u8glib with m2tklib */
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  /* assign u8g font to index 0 */
  m2.setFont(0, u8g_font_7x13);
}

void loop() {
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}


