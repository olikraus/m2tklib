/*

  SimpleTest.pde

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

  Just test a 16x4 LCD display

*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setCursor(uint8_t x, uint8_t y)
{
  if ( y >= 2 )
  {
    x+=16;
    y-=2;
  }
  lcd.setCursor(x, y);
}

void setup() {
  lcd.begin(16, 4);
  lcd.print("Simple Test");
}

void loop() {
  setCursor(0, 1);
  lcd.print("(0,1)");
  
  setCursor(0, 2);
  lcd.print("(0,2)");

  setCursor(8, 2);
  lcd.print("(8,2)");
  
  setCursor(0, 3);
  lcd.print(millis()/100);
}
