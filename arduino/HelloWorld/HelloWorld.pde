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
#include "m2.h"
#include "m2ghdogm.h"

int a0Pin = 9;      // address line a0 for the dogm module

// initialize the dogm library
Dogm dogm(a0Pin);

// create a label 
M2_LABEL(hello_world_label, "", "Hello World!");

void setup() {
  m2_Init(&hello_world_label, NULL, NULL, m2_gh_dogm_fbs);	
}

void loop() {
    dogm.start();
    do{
      m2_Draw();
    } while( dogm.next() );
}


