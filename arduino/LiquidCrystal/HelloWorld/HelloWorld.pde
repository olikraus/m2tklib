/*

  HelloWorld.pde
  
  Character LCD Example

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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setCursor(uint8_t x, uint8_t y)
{
  /* translate the x-y position from the m2tklib to the LiquidCrystal lib */
  /* assume 16x4 display */
  
  y = 3-y;
  
  if ( y >= 2 )
  {
    x+=16;
    y-=2;
  }
  
  lcd.setCursor(x, y);
}

uint8_t m2_gh_liquid_crystal(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:		
      lcd.begin(16, 4);
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      setCursor(arg->x, arg->y);
      lcd.print(arg->s);
      break;
  }
  return m2_gh_dummy(arg);
}

M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&hello_world_label, NULL, NULL, m2_gh_liquid_crystal);

void setup() {
}

void loop() {
  m2.draw();
}


