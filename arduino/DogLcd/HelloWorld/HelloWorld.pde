/*

  HelloWorld.pde
  
  DogLcd 16x2 example

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

#include <DogLcd.h>
#include "M2tk.h"
#include "m2ghdoglcd.h"

DogLcd lcd(5, 7, 29, 1, -1, 3);

M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&hello_world_label, NULL, NULL, m2_gh_doglcd);

void setup() {
  // 1. Assign the DogLcd object to m2tklib
  m2_SetDogLcd(&lcd, DOG_LCD_M162,0x25,DOG_LCD_VCC_3V3);
  // 2. Set the backlight (sequence is important!) 
  lcd.setBacklight(1, 0);
}

void loop() {
  m2.draw();
  delay(500);
}


