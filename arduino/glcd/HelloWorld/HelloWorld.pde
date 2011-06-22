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

#include <glcd.h>
//#include "M2tk.h"
#include "m2.h"
#include "m2ghglcd.h"


extern "C" uint8_t m2_gh_glcd_bf_yyy(m2_gfx_arg_p  arg)
{
  return 0;
}


M2_LABEL(hello_world_label, NULL, "Hello World");
//M2tk m2(&hello_world_label, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_bf);
//M2tk m2(&hello_world_label, NULL, NULL, m2_gh_glcd_bf_yyy);


void setup() {
  //m2.draw();
  m2_Init(&hello_world_label, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_bf);
      GLCD.Init(NON_INVERTED);   // initialise the library, non inverted writes pixels onto a clear screen
      GLCD.ClearScreen();  
      GLCD.FillRect(0, 0, 64, 61, BLACK); // rectangle in left side of screen
}

void loop() {
}


