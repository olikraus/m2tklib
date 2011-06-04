/*

  HelloWorld.pde

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

int a0Pin = 9;
Dogm dogm(a0Pin);

M2_LABEL(hello_world_label, NULL, "Hello World");
M2tk m2(&hello_world_label, NULL, NULL, m2_gh_dogm_fbs);

void setup() {
  m2.setFont(0, font_7x13);
}

void loop() {
  dogm.start();
  do{
    m2.draw();
  } while( dogm.next() );
}


