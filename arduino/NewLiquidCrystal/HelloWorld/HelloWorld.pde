/*

  HelloWorld.pde
  
  New LiquidCrystal 16x2 example

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2013  olikraus@gmail.com

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

/*
    New LiquidCrystal 
    https://bitbucket.org/fmalpartida/new-liquidcrystal/
  
    Constructor				Include
    
    LiquidCrystal			#include <LiquidCrystal.h>
    LiquidCrystal_I2C		#include <LiquidCrystal_I2C.h>
    LiquidCrystal_SR			#include <LiquidCrystal_SR.h>
    LiquidCrystal_SR2W		#include <LiquidCrystal_SR2W.h>
    LiquidCrystal_SR3W		#include <LiquidCrystal_SR3W.h>
*/

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghnlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&hello_world_label, NULL, NULL, m2_gh_nlc);

void setup() {
  m2_SetNewLiquidCrystal(&lcd, 16, 2);
}

void loop() {
  m2.draw();
  delay(500);
}


